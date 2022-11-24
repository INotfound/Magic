#pragma once
#include <string>
#include <stdexcept>
#include <cstring>

class MultiPartParser {
public:
	typedef void (*Callback)(const char *buffer, size_t start, size_t end, void *userData);
private:
    #undef ERROR
    enum State {
        ERROR,
        START,
        START_BOUNDARY,
        HEADER_FIELD_START,
        HEADER_FIELD,
        HEADER_VALUE_START,
        HEADER_VALUE,
        HEADER_VALUE_ALMOST_DONE,
        HEADERS_ALMOST_DONE,
        PART_DATA_START,
        PART_DATA,
        PART_END,
        END
    };

    enum Flags {
        PART_BOUNDARY = 1,
        LAST_BOUNDARY = 2
    };
public:
    Callback onPartBegin;
    Callback onHeaderField;
    Callback onHeaderValue;
    Callback onHeaderEnd;
    Callback onHeadersEnd;
    Callback onPartData;
    Callback onPartEnd;
    Callback onEnd;
    void *userData;

    MultiPartParser();

    explicit MultiPartParser(const std::string &boundary);

    ~MultiPartParser();

    void reset();

    bool stopped() const;

    bool hasError() const;

    bool succeeded() const;

    const char* getErrorMessage() const;

    size_t feed(const char *buffer, size_t len);

    void setBoundary(const std::string &boundary);
private:
	void resetCallbacks();
	
	void indexBoundary();
	
	void callback(Callback cb, const char *buffer = nullptr, size_t start = UNMARKED, size_t end = UNMARKED, bool allowEmpty = false) const;
	
	void dataCallback(Callback cb, size_t &mark, const char *buffer, size_t i, size_t bufferLen, bool clear, bool allowEmpty = false);
	
	static char lower(char c);
	
	inline bool isBoundaryChar(char c) const;
	
	static bool isHeaderFieldCharacter(char c);
	
	void setError(const char *message);
	
	void processPartData(size_t &prevIndex, size_t &index, const char *buffer,
		size_t len, size_t boundaryEnd, size_t &i, char c, State &state, int &flags);
private:
    std::string boundary;
    const char *boundaryData;
    size_t boundarySize;
    bool boundaryIndex[256];
    char *lookbehind;
    size_t lookbehindSize;
    State state;
    int flags;
    size_t index;
    size_t headerFieldMark;
    size_t headerValueMark;
    size_t partDataMark;
    const char *errorReason;

    static const char CR     = 13;
    static const char LF     = 10;
    static const char SPACE  = 32;
    static const char HYPHEN = 45;
    static const char COLON  = 58;
    static const size_t UNMARKED = (size_t) -1;
};
