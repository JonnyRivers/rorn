#include "../../Interfaces/StreamReader.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

StreamReader::StreamReader(Stream* stream) : stream_(stream)
{
}

StreamReader::~StreamReader()
{
	delete stream_;
}

StreamReader::StreamReader(StreamReader& rhs)
{
	stream_ = rhs.stream_;
	rhs.stream_ = 0;
}

StreamReader& StreamReader::operator=(StreamReader& rhs)
{
	stream_ = rhs.stream_;
	rhs.stream_ = 0;

	return *this;
}

void StreamReader::ReadData(void* buffer, int length)
{
	stream_->Read(buffer, length);
}

float StreamReader::ReadFloat()
{
	float value;
	stream_->Read(&value, sizeof(value));

	return value;
}

void StreamReader::ReadFloat4(Float4& float4)
{
	stream_->Read(&float4, sizeof(float4));
}

int StreamReader::ReadInt()
{
	int value;
	stream_->Read(&value, sizeof(value));

	return value;
}

unsigned int StreamReader::ReadUInt()
{
	unsigned int value;
	stream_->Read(&value, sizeof(value));

	return value;
}
