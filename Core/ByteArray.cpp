#include "ByteArray.h"

namespace Magic {
	ByteArray::~ByteArray(){
	}

	ByteArray::ByteArray(uint64_t size)
		:m_Size(size), m_Capacity(size){
		m_Data.reserve(m_Capacity + 1);
	}

	ByteArray::ByteArray(const char* data, uint64_t len)
		:m_Size(len),m_Capacity(len){
		m_Data.reserve(m_Capacity + 1);
		this->add(data, len);
	}

	ByteArray& ByteArray::append(const char* str, uint64_t len){
		this->add(str, len);
		return *this;
	}

	ByteArray& ByteArray::append(const std::string& str, uint64_t len){
		this->add(str.c_str(), len);
		return *this;
	}

	uint64_t ByteArray::getSize() const{
		return m_Size;
	}

	uint64_t ByteArray::getCapacity() const{
		return m_Capacity;
	}

	char* ByteArray::data(){
		return m_Data.data();
	}

	const char* ByteArray::data() const{
		return m_Data.data();
	}

	char ByteArray::at(uint64_t index) const{
		return m_Data.at(index);
	}

	int8_t ByteArray::toInt8(){
		int8_t value{0};
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	int16_t ByteArray::toInt16(){
		int16_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	int32_t ByteArray::toInt32(){
		int32_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	int64_t ByteArray::toInt64(){
		int64_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	uint8_t ByteArray::toUint8(){
		uint8_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	uint16_t ByteArray::toUint16(){
		uint16_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	uint32_t ByteArray::toUint32(){
		uint32_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	uint64_t ByteArray::toUint64(){
		uint64_t value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	float ByteArray::toFloat(){
		float value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	double ByteArray::toDouble(){
		double value{ 0 };
		to(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	std::string ByteArray::toString(){

	}

	ByteArray& ByteArray::setInt8(int8_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setInt16(int16_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setInt32(int64_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setInt64(int64_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setUint8(uint8_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setUint16(uint16_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setUint32(uint32_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setUint64(uint64_t value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setFloat(float value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	ByteArray& ByteArray::setDouble(double value){
		this->add(reinterpret_cast<char*>(&value), sizeof(value));
		return *this;
	}

	void ByteArray::to(char* value, uint64_t len){
	}

	void ByteArray::add(const char* value, uint64_t len){
	}

}