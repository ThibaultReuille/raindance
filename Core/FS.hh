#pragma once

#include <raindance/Core/Headers.hh>

class FS
{
public:

	class File
	{
	public:
		File(const char* filename)
		: m_FileName(filename)
		{
		}

		virtual ~File() {}

		virtual void load(const char* filename) = 0;

	private:
		std::string m_FileName;
	};

	class TextFile : public File
	{
	public:
		TextFile(const char* filename) : File(filename)
		{
			load(filename);
		}

		virtual void load(const char* filename)
		{
			std::ifstream f(filename);
			m_Content.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
		}

		virtual void dump()
		{
			LOG("%s\n", m_Content.c_str());
		}

		inline const std::string& content()	{ return m_Content; }

	private:
		std::string m_Content;
	};

	class BinaryFile : public File
	{
	public:
		BinaryFile(const char* filename) : File(filename)
		{
			load(filename);
		}

		virtual void load(const char* filename)
		{
			std::ifstream f(filename, std::ios::binary);
			m_Content.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
		}

		virtual void dump()
		{
			// TODO : Hexdump
		}

		inline const std::vector<char>& content() const	{ return m_Content; }
		inline const char* data() const { return m_Content.data(); }

	private:
		std::vector<char> m_Content;
	};
};