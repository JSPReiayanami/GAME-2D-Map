
#include"TabFileReader.h"

// ---------------------------------------------------
// ################ 全局函数定义 开始 ################
// ################ 全局函数定义 结束 ################
// ---------------------------------------------------
// ################ 本地常量定义 开始 ################
// ################ 本地常量定义 结束 ################
// ---------------------------------------------------
// ################ 全局对象定义 开始 ################
//G_DEFINE_BEGIN;
//G_DEFINE_CTOR;
//G_DEFINE_DTOR;
//G_DEFINE_END;
// ################ 全局对象定义 结束 ################
// ---------------------------------------------------

static size_t getLine( const char* data ) {
	size_t pos = 0;
	while ( data[pos] != '\0' && data[pos] != '\r' && data[pos] != '\n' ) ++pos;
	return pos;
}

static size_t getTab( const char* data ) {
	size_t pos = 0;
	while ( data[pos] != '\0' && data[pos] != '\t' && data[pos] != '\r' && data[pos] != '\n' ) ++pos;
	return pos;
}

TFReader::TabFileReaderEx::TabFileReaderEx() {
	refcount = 0;
	filesize = 0;
	filedata = NULL;
	recordset = NULL;
}

TFReader::TabFileReaderEx::~TabFileReaderEx() {
	CC_SAFE_DELETE( filedata );
	CC_SAFE_DELETE( recordset );
}

void TFReader::TabFileReaderEx::retain() {
	refcount++;
}

void TFReader::TabFileReaderEx::release() {
	if ( --refcount == 0 ) 
		delete this; 
}

#ifdef USING_FIXED_TEMPLATE_OBJECT
TFReader::const_ref_string::const_ref_string( 
	TabFileReaderEx *ref, const char* const value ) : const_string( value ), ref( ref ) {
	if ( ref != NULL )
		ref->retain();
}

TFReader::const_ref_string::const_ref_string( 
	const const_ref_string &crs ) : const_string( crs.c_str() ), ref( crs.ref ) {
	if ( ref != NULL )
		ref->retain();
}

TFReader::const_ref_string& TFReader::const_ref_string::operator = ( const const_ref_string &other ) {
	clear();
	const_ref_string *place = this;
	place = new ( place ) const_ref_string ( other.ref, other.c_str() );
	return *this;
}

void TFReader::const_ref_string::clear() {
	if ( ref != NULL ) {
		ref->release();
		ref = NULL;
	}
}

TFReader::const_ref_string::~const_ref_string() { 
	if ( ref ) ref->release(); 
}
#endif

TFReader TFReader::create( const char* filename ) {
	CCLOG( "loading the tabfile %s", filename );

	Data file_data = FileUtils::getInstance()->getDataFromFile(filename);
	unsigned char *data = file_data.getBytes();
	ssize_t size = file_data.getSize();
	if ( size == 0 ) {
		file_data.clear();
		return TFReader();
	}

	if ( size > 2 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF )
		size -= 3; // BOM header
	else {
		CCLOG( "File %s Error is not UTF8", filename );
		file_data.clear();
		return TFReader();
	}

	TFReader ret = TFReader::create( data + 3, size, filename );
	file_data.clear();
	return ret;
}

TFReader TFReader::create( const unsigned char* data, size_t length, const char* filename ) {
	unsigned long size = length;
	TabFileReaderEx *tfr = new TabFileReaderEx;
	tfr->filename = filename;
	tfr->filesize = size;
	tfr->filedata = new char[ size+1 ];
	memcpy( const_cast< char* >( tfr->filedata ), data, size );
	const_cast< char* >( tfr->filedata )[ size ] = 0;

	char *filedata = const_cast< char* >( tfr->filedata );
	size_t offset = 0;
	size_t lineIdx = 0;
	//std::vector< const char* > vec;

	// 第一遍，表头数据初始化，和统计所有的有效数据行数，为矩阵索引做好内存分配准备
	size_t validCount = 0;
	size_t validValueLineOffset = 0;
	while ( offset < size ) {
		char *line = filedata + offset;
		++lineIdx;

		// 查找 \r\n 或 \r 或 \n，获取整行结束点
		// 由于filedata必定以\0结束，所以getLine必然不会获取一个超出长度的结果
		// 同时只有以上3种换行方法，所以处理逻辑如下
		size_t len = getLine( line ); 
		size_t lineSegment = offset;
		offset += len;
		if ( filedata[ offset ] == '\n' )  
			offset++;
		else if ( filedata[ offset ] == '\r' ) {
			offset++;
			if ( filedata[ offset ] == '\n' )
				offset++;
		}

		if ( *line == '\0' || *line == '#' || *line == '\r' || *line == '\n' )
			continue;

		bool isFieldset = ( *line == '$' );
		char *lineEnd = line + len;
		size_t idx = 0;
		size_t tab = 0;
		if ( isFieldset ) {
			++line; // 略过$标记，不计入表头里
			do { 
				tab = getTab( line );
				if ( tab >= 128 ) {
					CCLOG( "[ERROR] TFReader %s : fieldname \"%s\" length >= 128", filename, line );
					delete tfr;
					return TFReader();
				}
				line[tab] = 0;
				if ( line[0] == '\0' ) {
					// 空数据只能出现在行末，属于对excel输出数据的容错
					while( ++line < lineEnd ) {
						if ( line[0] != '\t' ) {
							if ( *lineEnd != '\0' )
								*lineEnd = '\0';
							CCLOG( "[ERROR] TFReader %s : find null fieldname at %d non-end line \"%s\"", filename, idx, line );
							delete tfr;
							return TFReader();
						}
					} 
					break;
				}
				if ( tfr->fieldset.find( line ) == tfr->fieldset.end() )
					tfr->fieldset[line] = idx++;
				else {
					CCLOG( "[ERROR] TFReader %s : fieldname \"%s\" is duplicate at %d and %d", 
						filename, line, tfr->fieldset[line], idx );
					idx++;
				}
				line += tab + 1;
			} while( line < lineEnd );
			if ( idx == 0 || idx != tfr->fieldset.size() ) {
				CCLOG( "[ERROR] TFReader %s : fieldcout is invalid idx=%d and fieldset=%d", 
					filename, idx, tfr->fieldset.size() );
				delete tfr;
				return TFReader();
			}
		} else {
			if ( tfr->fieldset.size() == 0 ) // 如果值在表头之前，就不处理了
				continue;
			if ( validValueLineOffset == 0 )
				validValueLineOffset = lineSegment;
			validCount++;
		}
	}

	if ( tfr->fieldset.size() == 0 || validValueLineOffset == 0 ) {
		CCLOG( "[ERROR] TFReader %s : tab file is empty", filename );
		delete tfr;
		return TFReader();
	}

	// 第二遍，填充矩阵索引
	tfr->itemcount = 0;
	tfr->recordset = new const char*[validCount*tfr->fieldset.size()];
	offset = validValueLineOffset;
	lineIdx = 0;
	while ( offset < size ) {
		
		char *line = filedata + offset;
		++lineIdx;

		// 查找 \r\n 或 \r 或 \n，获取整行结束点
		// 由于filedata必定以\0结束，所以getLine必然不会获取一个超出长度的结果
		// 同时只有以上3种换行方法，所以处理逻辑如下
		size_t len = getLine( line ); 
		offset += len;
		if ( filedata[ offset ] == '\n' )  
			filedata[ offset++ ] = 0;
		else if ( filedata[ offset ] == '\r' ) {
			filedata[ offset++ ] = 0;
			if ( filedata[ offset ] == '\n' )
				filedata[ offset++ ] = 0;
		}

		if ( *line == '\0' || *line == '#' )
			continue;

		if ( tfr->itemcount > validCount ) {
			delete tfr;
			CCLOG( "[ERROR] TFReader %s : validLineCount is %d limited, but a new readItemLine is appeared", 
				filename, validCount );
			return TFReader();
		}

		size_t itemSegment = tfr->itemcount++ * tfr->fieldset.size();

		char *lineEnd = line + len;
		size_t idx = 0;
		size_t tab = 0;
		do { 
			tab = getTab( line );
			line[tab] = 0;
			tfr->recordset[ itemSegment + idx++ ] = line;
			line += tab + 1;
		} while( line < lineEnd && idx < tfr->fieldset.size() );
		if ( idx < tfr->fieldset.size() || line < lineEnd ) {
			//CCLOG( "[ERROR] TFReader %s : line %d is not match fields count %d ( value count = %d )", 
			//	filename, lineIdx, tfr->fieldset.size(), idx );
			for ( ; idx < tfr->fieldset.size(); idx++ )
				tfr->recordset[ itemSegment + idx ] = NULL;
		}
	}
	if ( validCount != tfr->itemcount ) {
		delete tfr;
		CCLOG( "[ERROR] TFReader %s : validLineCount is %d but readItemLine is %d", 
			filename, validCount, tfr->itemcount );
		return TFReader();
	}
	return TFReader( tfr );
}

const char * TFReader::TabFileReaderEx::GetItem( size_t idIndex, const char* fieldName ) {
	std::map< const_string, unsigned int >::iterator it = fieldset.find( fieldName );
	if ( it == fieldset.end() ) {
	   CCLOG( "[ERROR] TFReader %s : fieldname \"%s\" not exit", filename.c_str(), fieldName );
	   return NULL;
	}
	//if ( idIndex >= recordset.size() ) {
	//   CCLOG( "[ERROR] TFReader %s : idIndex %d out of range %d", filename.c_str(), idIndex, recordset.size() );
	//   return NULL;
	//}
	//const vector< const char * > &vec = recordset.at( idIndex );
	//if ( it->second >= vec.size() ) {
	//   CCLOG( "[CRITICAL ERROR] TFReader %s : subIndex %d out of range %d", filename.c_str(), it->second, vec.size() );
	//   return NULL;
	//}
	//return vec.at( it->second );
	if ( idIndex >= itemcount ) {
	   CCLOG( "[ERROR] TFReader %s : idIndex %d out of range %d", filename.c_str(), idIndex, itemcount );
	   return NULL;
	}
	if ( it->second >= fieldset.size() ) {
	   CCLOG( "[CRITICAL ERROR] TFReader %s : subIndex %d out of range %d", filename.c_str(), it->second, fieldset.size() );
	   return NULL;
	}
	return recordset[ idIndex*fieldset.size()+it->second ];
}

size_t TFReader::GetItemCount() {
	//return ( *this ) ? reader->recordset.size() : 0;
	return ( *this ) ? reader->itemcount : 0;
}

bool TFReader::ContainsKey( const char* fieldName ) {
	if ( reader == NULL )
		return false;
	return reader->fieldset.find( fieldName ) != reader->fieldset.end();
}

const char* TFReader::GetItem( int idIndex, const char* fieldName ) {
	return ( *this ) ? reader->GetItem( idIndex, fieldName ) : NULL;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, std::string& result ) {
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL )
		return false;
	result = value;
	return true;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, uint32_t& result ) {
	result = 0;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' )
		return false;
	result = atoi( value );
	return true;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, int32_t& result ) {
	result = 0;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' )
		return false;
	result = atoi( value );
	return true;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, uint16_t& result ) {
	result = 0;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' )
		return false;
	result = atoi( value );
	return true;
}
#define PRIu64       "I64u"
bool TFReader::Read( size_t idIdx, const char* fieldName, uint64_t& result ) {
	result = 0;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' )
		return false;
	return sscanf(value, "%"PRIu64, &result) == 1;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, float& result ) {
	result = 0;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' )
		return false;
	result = atof( value );
	return true;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, bool& result ) {
	result = false;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' || *value == '0')
		return false;
	if (*value == 'T' || *value == 't' || *value == 'Y' || *value == 'y')
		result = true;
	else if (*value == 'F' || *value == 'f' || *value == 'N' || *value == 'n')
		result = false;
	else {
		CCLOG("BAD TRUE FALSE file: %s field: %s idx: %d", reader->filename.c_str(), fieldName, idIdx);
	}
	return true;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, Point& result ) {
	result.x = 0;
	result.y = 0;
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL || *value == '\0' )
		return false;
	result.x = atoi( value );
	while ( *value != '\0' && *value != ',' ) value++;
	if ( *value != ',' )
		return false;
    result.y = atoi( value+1 );
	return true;
}


bool TFReader::Read(size_t idIdx, const char* fieldName, std::vector<uint32_t>& list) {
	uint32_t n = 0;
	const char *value = GetItem(idIdx, fieldName);
	if (value == NULL || *value == '\0')
		return false;
	while (true) {
		n = (uint32_t)atoi(value);
		list.push_back(n);
		while (*value != '\0' && *value != ',') value++;
		if (*value == '\0')
			break;
		if (*value == ',')
			value++;
	}
	return true;
}

#ifdef USING_FIXED_TEMPLATE_OBJECT
bool TFReader::Read(size_t idIdx, const char* fieldName, BaseFixedArray<uint32_t>& list) {
	uint32_t n = 0;
	const char *value = GetItem(idIdx, fieldName);
	if (value == NULL || *value == '\0')
		return false;
	while (true) {
		n = (uint32_t)atoi(value);
		try {
			list.push_back(n);
		} catch ( exception &e ) {
			CCLOG( "CATCHED EXCEPTION !!! what = %s", e.what() );
			return false;
		}
		while (*value != '\0' && *value != ',') value++;
		if (*value == '\0')
			break;
		if (*value == ',')
			value++;
	}
	return true;
}

bool TFReader::Read( size_t idIdx, const char* fieldName, const_ref_string& result ) {
	const char *value = GetItem( idIdx, fieldName );
    if ( value == NULL )
		return false;
	result = const_ref_string( reader, value );
	return true;
}
#endif

bool TFReader::Read(size_t idIdx, const char* fieldName, Color3B& color3B) {
	uint32_t n = 0;
	const char *value = GetItem(idIdx, fieldName);
	if (value == NULL || *value == '\0')
		return false;

	GLubyte color[3] = { 255, 255, 255 };
	int i = 0;
	while (true) {
		n = (uint32_t)atoi(value);
		color[i++] = (GLubyte)n;
		while (*value != '\0' && *value != ',') value++;
		if (*value == '\0')
			break;
		if (*value == ',')
			value++;
		if (i >= 3)
			break;
	}

	if (i != 3)
		return false;
	
	color3B.r = color[0];
	color3B.g = color[1];
	color3B.b = color[2];
	return true;
}

bool TFReader::Read(size_t idIdx, const char* fieldName, Color4B& color4B) {
	uint32_t n = 0;
	const char *value = GetItem(idIdx, fieldName);
	if (value == NULL || *value == '\0')
		return false;

	GLubyte color[4] = { 255, 255, 255, 255 };
	int i = 0;
	while (true) {
		n = (uint32_t)atoi(value);
		color[i++] = (GLubyte)n;
		while (*value != '\0' && *value != ',') value++;
		if (*value == '\0')
			break;
		if (*value == ',')
			value++;
		if (i >= 4)
			break;
	}

	if (i != 4)
		return false;
	
	color4B.r = color[0];
	color4B.g = color[1];
	color4B.b = color[2];
	color4B.a = color[3];
	return true;
}
bool TFReader::Read(size_t idIdx, const char* fieldName, WeekDay& day){
	uint32_t n = 0;
	const char *value = GetItem(idIdx, fieldName);
	if (value == NULL || *value == '\0')
		return false;

	day.days = 0;
	int i = 0;
	while (true) {
		n = (uint32_t)atoi(value);
		if(n >= 7)//(合法数字是 0 - 6)
			return false;
		if((day.days & (1 << n)) != 0)
			return false;
		day.days = (day.days | (1 << n));
		i++;
		while (*value != '\0' && *value != ',') value++;
		if (*value == '\0')
			break;
		if (*value == ',')
			value++;
		if (i >= 7)
			break;
	}
	return true;
}
TFReader::TFReader() : reader( NULL ) {
}

TFReader::TFReader( TabFileReaderEx *tfr ) : reader( tfr ) { 
	if ( reader != NULL ) 
		reader->retain(); 
}

TFReader::TFReader( const TFReader &tfr ) : reader( tfr.reader ) { 
	if ( reader != NULL ) 
		reader->retain(); 
}

TFReader::~TFReader() { 
	if ( reader != NULL )
		reader->release();
}

TFReader& TFReader::operator = ( const TFReader &tfr ) {
	if ( reader != NULL && --reader->refcount == 0 ) 
		delete reader; 
	reader = tfr.reader;
	if ( reader != NULL ) 
		reader->refcount++; 
	return *this;
}

////////////////////////////////////////////////////////////
uint32_t StrReader::SplitCount() {
	size_t end = context.find_first_of(",", 0);
	if (end == std::string::npos)
		end = context.length();
	int count = 0;
	const char *it = context.c_str();
	for (size_t i = 0; i < end; i++, it++) {
		if (*it == ':')
			count++;
	}
	return count + 1;
}

StrReader StrReader::ReadBlock(const char *throwReason) {
	std::string block;
	size_t pos = std::string::npos;
	if (offset >= context.length())
		goto _err_return;
	pos = context.find_first_of(",", offset);
	if (pos != std::string::npos) {
		block = context.substr(offset, pos - offset);
		offset = pos + 1;
	}
	else {
		block = context.substr(offset);
		offset = context.length();
	}
	return StrReader(block);
_err_return:
	offset = context.length();
	if (throwReason != NULL)
		throw exception(throwReason);
	return StrReader();
}

StrReader& StrReader::ReadBlock(std::string &out, const char *throwReason) {
	size_t pos = std::string::npos;
	if (offset >= context.length())
		goto _err_return;
	pos = context.find_first_of(",", offset);
	if (pos != std::string::npos) {
		out = context.substr(offset, pos - offset);
		offset = pos + 1;
	}
	else {
		out = context.substr(offset);
		offset = context.length();
	}
	return *this;
_err_return:
	offset = context.length();
	if (throwReason != NULL)
		throw exception(throwReason);
	return *this;
}

StrReader& StrReader::Get(std::string &value, const char *throwReason) {
	size_t pos = std::string::npos;
	bool endSkip = false;
	if (offset >= context.length() || context[offset] == ',')
		goto _err_return;
	pos = context.find_first_of(",:", offset);
	if (pos == std::string::npos)
		pos = context.length();
	else if (context[pos] != ',')
		endSkip = true;
	value = context.substr(offset, pos - offset);
	offset = endSkip ? (pos + 1) : pos;
	return *this;
_err_return:
	offset = context.length();
	if (throwReason != NULL)
		throw exception(throwReason);
	return *this;
}

StrReader& StrReader::Get(uint32_t &value, const char *throwReason) {
	size_t pos = std::string::npos;
	bool endSkip = false;
	if (offset >= context.length() || context[offset] == ',')
		goto _err_return;
	pos = context.find_first_of(",:", offset);
	if (pos == std::string::npos)
		pos = context.length();
	else if (context[pos] != ',')
		endSkip = true;
	value = atoi(context.substr(offset, pos - offset).c_str());
	offset = endSkip ? (pos + 1) : pos;
	return *this;
_err_return:
	offset = context.length();
	if (throwReason != NULL)
		throw exception(throwReason);
	return *this;
}

StrReader& StrReader::Get(int32_t &value, const char *throwReason) {
	size_t pos = std::string::npos;
	bool endSkip = false;
	if (offset >= context.length() || context[offset] == ',')
		goto _err_return;
	pos = context.find_first_of(",:", offset);
	if (pos == std::string::npos)
		pos = context.length();
	else if (context[pos] != ',')
		endSkip = true;
	value = atoi(context.substr(offset, pos - offset).c_str());
	offset = endSkip ? (pos + 1) : pos;
	return *this;
_err_return:
	offset = context.length();
	if (throwReason != NULL)
		throw exception(throwReason);
	return *this;
}


StrReader& StrReader::Get(float &value, const char *throwReason) {
	size_t pos = std::string::npos;
	bool endSkip = false;
	if (offset >= context.length() || context[offset] == ',')
		goto _err_return;
	pos = context.find_first_of(",:", offset);
	if (pos == std::string::npos)
		pos = context.length();
	else if (context[pos] != ',')
		endSkip = true;
	value = atof(context.substr(offset, pos - offset).c_str());
	offset = endSkip ? (pos + 1) : pos;
	return *this;
_err_return:
	offset = context.length();
	if (throwReason != NULL)
		throw exception(throwReason);
	return *this;
}

RowFileReader::RowFileReader() {
	m_data = NULL;
	m_size = 0;
}

RowFileReader::~RowFileReader() {
	CC_SAFE_DELETE_ARRAY(m_data);
}

bool RowFileReader::Load(const char* fileName) {
	
	Data file_data = FileUtils::getInstance()->getDataFromFile(fileName);
	unsigned long size = file_data.getSize();
	unsigned char *data = file_data.getBytes();
	if (size == 0) {
		file_data.clear();
		return false;
	}

	if (size > 2 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF)
		size -= 3; // BOM header
	else {
		CCLOG("File %s Error is not UTF8", fileName);
		file_data.clear();
		return false;
	}

	m_data = new char[size + 1];
	memcpy(m_data, data + 3, size);
	m_data[size] = '\0';
	m_size = size;
	file_data.clear();

	int offset = size;
	while (offset-- > 0) {
		if (m_data[offset] == '\r' || m_data[offset] == '\n')
			m_data[offset] = 0;
	}
	return true;
}

static size_t GetTab(const char* data, size_t maxSize) {
	size_t pos = 0;
	while (data[pos] != '\t' && data[pos] != '\0' && pos < maxSize)
		++pos;
	return pos;
}

bool RowFileReader::EndOfFile(char* start) {
	return start >= m_data + m_size;
}

int RowFileReader::ReadLine(char*& start, const char* items[], int count) {
	if (start == NULL)
		start = m_data;

	const char* end = m_data + m_size;
	while ((*start == '\0' || *start == '#' || *start == '\t') && start < end) {
		if (*start == '#') {
			start++;
			while (*(++start) != '\0')
				;
		}
		else if (*start == '\0') {
			start++;
		}
		else if (*start == '\t') {
			start++;
		}
	}

	if (start >= end)
		return -1;

	char* cur = start;
	int n = 0;
	while (true) {
		if (cur >= end || n >= count)
			break;
		items[n++] = cur;
		size_t offset = GetTab(cur, m_size);
		if (cur[offset] == '\0') {
			start = cur + offset;
			return n;
		}
		else if (cur[offset] == '\t') {
			cur[offset] = '\0';
		}
		cur += (offset + 1);
		start = cur;
	}

	return n;
}
