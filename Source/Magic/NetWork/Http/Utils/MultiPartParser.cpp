#include <stdexcept>
#include <cstring>
#include "Magic/NetWork/Http/Utils/MultiPartParser.h"

MultiPartParser::MultiPartParser(){
    lookbehind = nullptr;
    resetCallbacks();
    reset();
}

MultiPartParser::MultiPartParser(const std::string_view& sv){
    lookbehind = nullptr;
    resetCallbacks();
    setBoundary(sv);
}

MultiPartParser::~MultiPartParser(){
    delete[] lookbehind;
}

void MultiPartParser::reset(){
    delete[] lookbehind;
    state = ERROR;
    boundary.clear();
    boundaryData = boundary.data();
    boundarySize = 0;
    lookbehind = nullptr;
    lookbehindSize = 0;
    flags = 0;
    index = 0;
    headerFieldMark = UNMARKED;
    headerValueMark = UNMARKED;
    partDataMark    = UNMARKED;
    errorReason     = "Parser uninitialized.";
}

bool MultiPartParser::stopped() const{
    return state == ERROR || state == END;
}

bool MultiPartParser::hasError() const{
    return state == ERROR;
}

bool MultiPartParser::succeeded() const{
    return state == END;
}

const char* MultiPartParser::getErrorMessage() const{
    return errorReason;
}

size_t MultiPartParser::feed(const char *buffer, size_t len){
    if (state == ERROR || len == 0){
        return 0;
    }

    State state         = this->state;
    int flags           = this->flags;
    size_t prevIndex    = this->index;
    size_t index        = this->index;
    size_t boundaryEnd  = boundarySize - 1;
    size_t i;
    char c, cl;

    for (i = 0; i < len; i++){
        c = buffer[i];

        switch (state){
            case ERROR:
                return i;
            case START:
                index = 0;
                state = START_BOUNDARY;
            case START_BOUNDARY:
                if (index == boundarySize - 2){
                    if (c != CR){
                        setError("Malformed. Expected CR after boundary.");
                        return i;
                    }
                    index++;
                    break;
                } else if (index - 1 == boundarySize - 2){
                    if (c != LF){
                        setError("Malformed. Expected LF after boundary CR.");
                        return i;
                    }
                    index = 0;
                    callback(onPartBegin);
                    state = HEADER_FIELD_START;
                    break;
                }
                if (c != boundary[index + 2]){
                    setError("Malformed. Found different boundary data than the given one.");
                    return i;
                }
                index++;
                break;
            case HEADER_FIELD_START:
                state = HEADER_FIELD;
                headerFieldMark = i;
                index = 0;
            case HEADER_FIELD:
                if (c == CR){
                    headerFieldMark = UNMARKED;
                    state = HEADERS_ALMOST_DONE;
                    break;
                }

                index++;
                if (c == HYPHEN){
                    break;
                }

                if (c == COLON){
                    if (index == 1){
                        // empty header field
                        setError("Malformed first header name character.");
                        return i;
                    }
                    dataCallback(onHeaderField, headerFieldMark, buffer, i, len, true);
                    state = HEADER_VALUE_START;
                    break;
                }

                cl = lower(c);
                if (cl < 'a' || cl > 'z'){
                    setError("Malformed header name.");
                    return i;
                }
                break;
            case HEADER_VALUE_START:
                if (c == SPACE){
                    break;
                }

                headerValueMark = i;
                state = HEADER_VALUE;
            case HEADER_VALUE:
                if (c == CR){
                    dataCallback(onHeaderValue, headerValueMark, buffer, i, len, true, true);
                    callback(onHeaderEnd);
                    state = HEADER_VALUE_ALMOST_DONE;
                }
                break;
            case HEADER_VALUE_ALMOST_DONE:
                if (c != LF){
                    setError("Malformed header value: LF expected after CR");
                    return i;
                }

                state = HEADER_FIELD_START;
                break;
            case HEADERS_ALMOST_DONE:
                if (c != LF){
                    setError("Malformed header ending: LF expected after CR");
                    return i;
                }

                callback(onHeadersEnd);
                state = PART_DATA_START;
                break;
            case PART_DATA_START:
                state = PART_DATA;
                partDataMark = i;
            case PART_DATA:
                processPartData(prevIndex, index, buffer, len, boundaryEnd, i, c, state, flags);
                break;
            default:
                return i;
        }
    }

    dataCallback(onHeaderField, headerFieldMark, buffer, i, len, false);
    dataCallback(onHeaderValue, headerValueMark, buffer, i, len, false);
    dataCallback(onPartData, partDataMark, buffer, i, len, false);

    this->index = index;
    this->state = state;
    this->flags = flags;

    return len;
}

void MultiPartParser::setBoundary(const std::string_view& sv){
    reset();
    this->boundary = "\r\n--";
    boundary.append(sv.data(),sv.size());
    boundaryData = this->boundary.data();
    boundarySize = this->boundary.size();
    indexBoundary();
    lookbehind = new char[boundarySize + 8];
    lookbehindSize = boundarySize + 8;
    state = START;
    errorReason = "No error.";
}

void MultiPartParser::resetCallbacks(){
        onPartBegin   = nullptr;
        onHeaderField = nullptr;
        onHeaderValue = nullptr;
        onHeaderEnd   = nullptr;
        onHeadersEnd  = nullptr;
        onPartData    = nullptr;
        onPartEnd     = nullptr;
        onEnd         = nullptr;
        userData      = nullptr;
    }

void MultiPartParser::indexBoundary(){
    const char *current;
    const char *end = boundaryData + boundarySize;

    memset(boundaryIndex, 0, sizeof(boundaryIndex));

    for (current = boundaryData; current < end; current++){
        boundaryIndex[(unsigned char) *current] = true;
    }
}

void MultiPartParser::callback(Callback cb, const char *buffer, size_t start,size_t end, bool allowEmpty) const{
    if (start != UNMARKED && start == end && !allowEmpty){
        return;
    }
    if (cb != nullptr){
        cb(buffer, start, end, userData);
    }
}

void MultiPartParser::dataCallback(Callback cb, size_t &mark, const char *buffer, size_t i, size_t bufferLen,bool clear, bool allowEmpty){
    if (mark == UNMARKED){
        return;
    }

    if (!clear){
        callback(cb, buffer, mark, bufferLen, allowEmpty);
        mark = 0;
    } else {
        callback(cb, buffer, mark, i, allowEmpty);
        mark = UNMARKED;
    }
}

char MultiPartParser::lower(char c){
    return c | 0x20;
}

inline bool MultiPartParser::isBoundaryChar(char c) const{
    return boundaryIndex[(unsigned char) c];
}

bool MultiPartParser::isHeaderFieldCharacter(char c){
    return (c >= 'a' && c <= 'z')
           || (c >= 'A' && c <= 'Z')
           || c == HYPHEN;
}

void MultiPartParser::setError(const char *message){
    state = ERROR;
    errorReason = message;
}

void MultiPartParser::processPartData(size_t &prevIndex, size_t &index, const char *buffer,
                     size_t len, size_t boundaryEnd, size_t &i, char c, State &state, int &flags){
    prevIndex = index;

    if (index == 0){
        // boyer-moore derived algorithm to safely skip non-boundary data
        while (i + boundarySize <= len){
            if (isBoundaryChar(buffer[i + boundaryEnd])){
                break;
            }

            i += boundarySize;
        }
        if (i == len){
            return;
        }
        c = buffer[i];
    }

    if (index < boundarySize){
        if (boundary[index] == c){
            if (index == 0){
                dataCallback(onPartData, partDataMark, buffer, i, len, true);
            }
            index++;
        } else {
            index = 0;
        }
    } else if (index == boundarySize){
        index++;
        if (c == CR){
            // CR = part boundary
            flags |= PART_BOUNDARY;
        } else if (c == HYPHEN){
            // HYPHEN = end boundary
            flags |= LAST_BOUNDARY;
        } else {
            index = 0;
        }
    } else if (index - 1 == boundarySize){
        if (flags & PART_BOUNDARY){
            index = 0;
            if (c == LF){
                // unset the PART_BOUNDARY flag
                flags &= ~PART_BOUNDARY;
                callback(onPartEnd);
                callback(onPartBegin);
                state = HEADER_FIELD_START;
                return;
            }
        } else if (flags & LAST_BOUNDARY){
            if (c == HYPHEN){
                callback(onPartEnd);
                callback(onEnd);
                state = END;
            } else {
                index = 0;
            }
        } else {
            index = 0;
        }
    } else if (index - 2 == boundarySize){
        if (c == CR){
            index++;
        } else {
            index = 0;
        }
    } else if (index - boundarySize == 3){
        index = 0;
        if (c == LF){
            callback(onPartEnd);
            callback(onEnd);
            state = END;
            return;
        }
    }

    if (index > 0){
        // when matching a possible boundary, keep a lookbehind reference
        // in case it turns out to be a false lead
        if (index - 1 >= lookbehindSize){
            setError("Parser bug: index overflows lookbehind buffer. "
                     "Please send bug report with input file attached.");
            throw std::out_of_range("index overflows lookbehind buffer");
        } else if (static_cast<int64_t>(index) - 1 < 0){
            setError("Parser bug: index underflows lookbehind buffer. "
                     "Please send bug report with input file attached.");
            throw std::out_of_range("index underflows lookbehind buffer");
        }
        lookbehind[index - 1] = c;
    } else if (prevIndex > 0){
        // if our boundary turned out to be rubbish, the captured lookbehind
        // belongs to partData
        callback(onPartData, lookbehind, 0, prevIndex);
        prevIndex = 0;
        partDataMark = i;

        // reconsider the current character even so it interrupted the sequence
        // it could be the beginning of a new sequence
        i--;
    }
}
