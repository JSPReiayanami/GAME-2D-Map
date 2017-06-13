#ifndef _TABFILEREADER_
#define _TABFILEREADER_

#include<iostream>
#include<fstream>
#include<stdio.h>
#include"cocos2d.h"
#include<vector>
#include<map>

USING_NS_CC;
using namespace std;

/*
class TabFileReader
{
public:
	static TabFileReader* creat(const char* filename);
	~TabFileReader();

protected:
	TabFileReader();
	void ReadStringToListRecord(string &str);
	void AddADictField(string &str);

protected:
	int m_listRecordCount;
	map<string,unsigned int>* m_pListRecord;
	vector<vector<string> > *m_pDictField;

public:
	int GetItemCount();
	const string& GetItem(int idIndex,const char* fieldName);

    void Read(int idIdx, const char* fieldName, std::string& result)
    {
        result = this->GetItem(idIdx, fieldName);
    }
    void Read(int idIdx, const char* fieldName, uint32_t& result)
    {
        result = atoi(this->GetItem(idIdx, fieldName).c_str());
    }

};
*/

#define TFREAD( _idx, _name, _recever ) reader.Read( _idx, #_name, _recever._name )
#define TFGET( _idx, _name ) reader.Read( _idx, #_name, _name )
typedef struct {
	unsigned char days;
	bool ContainsDay(uint32_t day) const{
	   if(day >= 7 ) return false;
	   return (days & (1 << day)) > 0 ? true : false;
	}
} WeekDay;

#define USING_FIXED_TEMPLATE_OBJECT

#ifdef USING_FIXED_TEMPLATE_OBJECT

#ifndef _CONST_STRING_
#define _CONST_STRING_
class const_string {
protected:
	const char* const value;
	mutable size_t size;
public:
	const_string() : value( NULL ), size( -1 ) {}
	const_string( const char* const value ) : value( value ), size( -1 ) {}
	const_string( const const_string &o ) : value( o.value ), size( o.size ) {}
	const_string& operator = ( const const_string &o ) { new ( this ) const_string( o.value ); size = o.size; return *this; }
	const char* const c_str() const { return value; }
	operator const char* const () const { return value; }
	bool empty() const { return value ? ( value[0] == 0 ) : true; }
	size_t length() const { if ( size == -1 ) size = value ? strlen( value ) : 0; return size; }
	bool operator < ( const const_string &o ) const { return strcmp( value, o.value ) < 0; }
	bool operator == ( const char* str ) const { return strcmp( value, str ) == 0; }
	bool operator == ( const std::string &str ) const { return strcmp( value, str.c_str() ) == 0; }
};
#endif

template < typename _T >
struct BaseFixedArray {
	virtual ~BaseFixedArray() {}
	virtual size_t size() const = 0;
	virtual size_t capacity() const = 0;
	virtual const _T& operator[] ( size_t idx ) const = 0;
	virtual void push_back( const _T &value ) = 0;
};

template < typename _T, size_t arraySize >
class FixedArray : public BaseFixedArray< _T > {
	static const size_t _capacity = arraySize;
	size_t _size;
	_T _array[arraySize];
public:
	FixedArray() : _size( 0 ) {}
	size_t size() const { return _size; }
	size_t capacity() const { return _capacity; }
	const _T& operator[] ( size_t idx ) const {
		if ( idx >= _size || idx >= _capacity )
			throw ccexception( "FixedArray out of range" );
		return _array[idx];
	}
	_T& operator[] ( size_t idx ) {
		if ( idx >= _size || idx >= _capacity )
			throw ccexception( "FixedArray out of range" );
		return _array[idx];
	}
	const _T& at ( size_t idx ) const {
		return (*this)[idx];
	}
	_T& at ( size_t idx ) {
		return (*this)[idx];
	}
	void resize( size_t size ) {
		if ( size > _capacity )
			throw ccexception( "FixedArray new size > capacity limited" );
		_size = size;
	}
	void push_back( const _T &value ) {
		if ( _size >= _capacity )
			throw ccexception( "FixedArray array is full" );
		_array[_size++] = value;
	}
};
#else
#define const_string std::string
#define BaseFixedArray std::vector
template < typename _T, size_t arraySize >
class FixedArray : public BaseFixedArray<_T> {
};
#endif

class TFReader {
public:
	static TFReader create( const char* filename );
	static TFReader create( const unsigned char* data, size_t length, const char* filename );

public:
	operator bool () { return reader != NULL; }

	size_t GetItemCount();
	bool ContainsKey( const char* fieldName );
	const char* GetItem( int idIndex, const char* fieldName );

	bool Read( size_t idIdx, const char* fieldName, std::string& result );
	bool Read( size_t idIdx, const char* fieldName, uint64_t& result );
	bool Read( size_t idIdx, const char* fieldName, uint32_t& result );
	bool Read( size_t idIdx, const char* fieldName, uint16_t& result );
	bool Read( size_t idIdx, const char* fieldName, int32_t& result );
	bool Read( size_t idIdx, const char* fieldName, Point& result );
	
	bool Read( size_t idIdx, const char* fieldName, bool& result );
	bool Read( size_t idIdx, const char* fieldName, float& result );

	bool Read(size_t idIdx, const char* fieldName, std::vector<uint32_t>& list);
#ifdef USING_FIXED_TEMPLATE_OBJECT
	bool Read(size_t idIdx, const char* fieldName, BaseFixedArray<uint32_t>& list);
#endif
	bool Read(size_t idIdx, const char* fieldName, Color3B& color3B);
	bool Read(size_t idIdx, const char* fieldName, Color4B& color4B);

	bool Read(size_t idIdx, const char* fieldName, WeekDay& day);
private:
	class TabFileReaderEx {
	public:
		TabFileReaderEx();
		~TabFileReaderEx();
		void retain();
		void release();
		const char* GetItem( size_t idIndex, const char* fieldName );
		std::string filename;
		uint32_t refcount;
		size_t filesize;
		const char* filedata; // 原始的表数据，只不过把tab替换成了\0
		std::map< const_string, unsigned int > fieldset; // 表头索引，用于标记某个字段所在的列序号
		//std::vector< std::vector< const char* > > recordset; // 以行列为序，矩阵形式存储的值
		size_t itemcount;
		const char** recordset; // 以行列为序，矩阵形式存储的值，行宽为fieldset.size()
	};

public:
#ifdef USING_FIXED_TEMPLATE_OBJECT
	class const_ref_string : public const_string {
		friend class TFReader;
		using const_string::value;
		TabFileReaderEx *ref;
		const_ref_string( TabFileReaderEx *ref, const char* const value );
	public:
		const_ref_string() : const_string( NULL ), ref( NULL ) {}
		const_ref_string( const char* str ) : const_string( str ), ref( NULL ) {}
		const_ref_string( const const_ref_string &crs );
		const_ref_string& operator = ( const const_ref_string &other );
		~const_ref_string();
		void clear();
	};
	bool Read( size_t idIdx, const char* fieldName, const_ref_string& result );
#endif
public:
	TFReader();
	TFReader( TabFileReaderEx *tfr );
	TFReader( const TFReader &tfr );
	~TFReader();
	TFReader& operator = ( const TFReader &tfr );

private:
	TabFileReaderEx *reader;
};

#ifdef USING_FIXED_TEMPLATE_OBJECT
typedef TFReader::const_ref_string template_string;
#else
typedef std::string template_string;
#endif
typedef const std::string& refstr;

class StrReader {
	uint32_t offset;
	std::string context;
public:
	struct ThrowError {};
	StrReader() : offset( 0 ) {}
	StrReader( refstr context ) : offset( 0 ), context( context ) {}
	void operator = ( const StrReader &other ) { offset = other.offset; context = other.context; }
	uint32_t SplitCount(); // 冒号分割
	StrReader ReadBlock( const char *throwReason = NULL ); // 逗号结尾
	StrReader& ReadBlock( std::string &out, const char *throwReason = NULL ); // 逗号结尾
	// 冒号分割
	StrReader& Get( std::string &value, const char *throwReason = NULL );
	StrReader& Get( uint32_t &value, const char *throwReason = NULL );
	StrReader& Get( int32_t &value, const char *throwReason = NULL );
	StrReader& Get( float &value, const char *throwReason = NULL );
};

/**
	文件规格：文件由若干行组成，每行由\t分隔。
	此模块从文件读取内容以后，保存在内存里，并将\r和\n全部替换为\0
	外部逻辑使用的时候，一次读取一行（即\0分隔的部分），并且将\t替换为\0
	所以，用这个类加载文件以后，只能使用一次。
*/
class RowFileReader {
public:
	RowFileReader();
	~RowFileReader();

	bool Load(const char* fileName);
	int ReadLine(char*& start, const char* items[], int count);
	bool EndOfFile(char* start);

private:
	char* m_data;
	size_t m_size;
};

#endif