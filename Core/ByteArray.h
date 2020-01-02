#pragma once
#include <string>
#include <vector>

namespace Magic {
	class ByteArray
	{
	public:
		~ByteArray();
		explicit ByteArray(uint64_t size);
		ByteArray(const char* data, uint64_t len);
		
		ByteArray& append(const char* str, uint64_t len);
		ByteArray& append(const std::string& str, uint64_t len);

		uint64_t getSize() const;
		uint64_t getCapacity() const;

		char* data();
		const char* data() const;
		char at(uint64_t i) const;

		int8_t		toInt8();
		int16_t		toInt16();
		int32_t		toInt32();
		int64_t		toInt64();
		uint8_t		toUint8();
		uint16_t	toUint16();
		uint32_t	toUint32();
		uint64_t	toUint64();
		float		toFloat();
		double		toDouble();
		std::string toString();

		ByteArray&  setInt8(int8_t value);
		ByteArray&  setInt16(int16_t value);
		ByteArray&  setInt32(int64_t value);
		ByteArray&  setInt64(int64_t value);
		ByteArray&  setUint8(uint8_t value);
		ByteArray&	setUint16(uint16_t value);
		ByteArray&	setUint32(uint32_t value);
		ByteArray&	setUint64(uint64_t value);
		ByteArray&  setFloat(float value);
		ByteArray&  setDouble(double value);
	private:
		void to(char* value, uint64_t len);
		void add(const char* value, uint64_t len);
	private:
		uint64_t m_Size{};
		uint64_t m_Position{};
		uint64_t m_Capacity{};
		std::vector<char> m_Data{};
	};

}