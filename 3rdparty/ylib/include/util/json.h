#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "base/define.h"
#include "base/conversion.h"
/* project version */
#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 7
#define CJSON_VERSION_PATCH 15

#include <stddef.h>

/* cJSON Types: */
#define cJSON_Invalid (0)
#define cJSON_False  (1 << 0)
#define cJSON_True   (1 << 1)
#define cJSON_NULL   (1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array  (1 << 5)
#define cJSON_Object (1 << 6)
#define cJSON_Raw    (1 << 7) /* raw json */

#define cJSON_IsReference 256
#define cJSON_StringIsConst 512

/* The cJSON structure: */
typedef struct cJSON
{
	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct cJSON* next;
	struct cJSON* prev;
	/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
	struct cJSON* child;

	/* The type of the item, as above. */
	int type;

	/* The item's string, if type==cJSON_String  and type == cJSON_Raw */
	char* valuestring;
	/* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
	int valueint;
	/* The item's number, if type==cJSON_Number */
	double valuedouble;

	/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
	char* string;
} cJSON;

typedef struct cJSON_Hooks
{
	/* malloc/free are CDECL on Windows regardless of the default calling convention of the compiler, so ensure the hooks allow passing those functions directly. */
	void* (*malloc_fn)(size_t sz);
	void (*free_fn)(void* ptr);
} cJSON_Hooks;

typedef int cJSON_bool;


#define CJSON_NESTING_LIMIT 1000

/* Supply malloc, realloc and free functions to cJSON */
void cJSON_InitHooks(cJSON_Hooks* hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of cJSON_Parse (with cJSON_Delete) and cJSON_Print (with stdlib free, cJSON_Hooks.free_fn, or cJSON_free as appropriate). The exception is cJSON_PrintPreallocated, where the caller has full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a cJSON object you can interrogate. */
cJSON* cJSON_Parse(const char* value);
cJSON* cJSON_ParseWithLength(const char* value, size_t buffer_length);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error so will match cJSON_GetErrorPtr(). */
cJSON* cJSON_ParseWithOpts(const char* value, const char** return_parse_end, cJSON_bool require_null_terminated);
cJSON* cJSON_ParseWithLengthOpts(const char* value, size_t buffer_length, const char** return_parse_end, cJSON_bool require_null_terminated);

/* Render a cJSON entity to text for transfer/storage. */
char* cJSON_Print(const cJSON* item);
/* Render a cJSON entity to text for transfer/storage without any formatting. */
char* cJSON_PrintUnformatted(const cJSON* item);
/* Render a cJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
char* cJSON_PrintBuffered(const cJSON* item, int prebuffer, cJSON_bool fmt);
/* Render a cJSON entity to text using a buffer already allocated in memory with given length. Returns 1 on success and 0 on failure. */
/* NOTE: cJSON is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need */
cJSON_bool cJSON_PrintPreallocated(cJSON* item, char* buffer, const int length, const cJSON_bool format);
/* Delete a cJSON entity and all subentities. */
void cJSON_Delete(cJSON* item);

/* Returns the number of items in an array (or object). */
int cJSON_GetArraySize(const cJSON* array);
/* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
cJSON* cJSON_GetArrayItem(const cJSON* array, int index);
/* Get item "string" from object. Case insensitive. */
cJSON* cJSON_GetObjectItem(const cJSON* const object, const char* const string);
cJSON* cJSON_GetObjectItemCaseSensitive(const cJSON* const object, const char* const string);
cJSON_bool cJSON_HasObjectItem(const cJSON* object, const char* string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
const char* cJSON_GetErrorPtr(void);

/* Check item type and return its value */
char* cJSON_GetStringValue(const cJSON* const item);
double cJSON_GetNumberValue(const cJSON* const item);

/* These functions check the type of an item */
cJSON_bool cJSON_IsInvalid(const cJSON* const item);
cJSON_bool cJSON_IsFalse(const cJSON* const item);
cJSON_bool cJSON_IsTrue(const cJSON* const item);
cJSON_bool cJSON_IsBool(const cJSON* const item);
cJSON_bool cJSON_IsNull(const cJSON* const item);
cJSON_bool cJSON_IsNumber(const cJSON* const item);
cJSON_bool cJSON_IsString(const cJSON* const item);
cJSON_bool cJSON_IsArray(const cJSON* const item);
cJSON_bool cJSON_IsObject(const cJSON* const item);
cJSON_bool cJSON_IsRaw(const cJSON* const item);

/* These calls create a cJSON item of the appropriate type. */
cJSON* cJSON_CreateNull(void);
cJSON* cJSON_CreateTrue(void);
cJSON* cJSON_CreateFalse(void);
cJSON* cJSON_CreateBool(cJSON_bool boolean);
cJSON* cJSON_CreateNumber(double num);
cJSON* cJSON_CreateString(const char* string);
/* raw json */
cJSON* cJSON_CreateRaw(const char* raw);
cJSON* cJSON_CreateArray(void);
cJSON* cJSON_CreateObject(void);

/* Create a string where valuestring references a string so
 * it will not be freed by cJSON_Delete */
cJSON* cJSON_CreateStringReference(const char* string);
/* Create an object/array that only references it's elements so
 * they will not be freed by cJSON_Delete */
cJSON* cJSON_CreateObjectReference(const cJSON* child);
cJSON* cJSON_CreateArrayReference(const cJSON* child);

/* These utilities create an Array of count items.
 * The parameter count cannot be greater than the number of elements in the number array, otherwise array access will be out of bounds.*/
cJSON* cJSON_CreateIntArray(const int* numbers, int count);
cJSON* cJSON_CreateFloatArray(const float* numbers, int count);
cJSON* cJSON_CreateDoubleArray(const double* numbers, int count);
cJSON* cJSON_CreateStringArray(const char* const* strings, int count);

/* Append item to the specified array/object. */
cJSON_bool cJSON_AddItemToArray(cJSON* array, cJSON* item);
cJSON_bool cJSON_AddItemToObject(cJSON* object, const char* string, cJSON* item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the cJSON object.
 * WARNING: When this function was used, make sure to always check that (item->type & cJSON_StringIsConst) is zero before
 * writing to `item->string` */
cJSON_bool cJSON_AddItemToObjectCS(cJSON* object, const char* string, cJSON* item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
cJSON_bool cJSON_AddItemReferenceToArray(cJSON* array, cJSON* item);
cJSON_bool cJSON_AddItemReferenceToObject(cJSON* object, const char* string, cJSON* item);

/* Remove/Detach items from Arrays/Objects. */
cJSON* cJSON_DetachItemViaPointer(cJSON* parent, cJSON* const item);
cJSON* cJSON_DetachItemFromArray(cJSON* array, int which);
void cJSON_DeleteItemFromArray(cJSON* array, int which);
cJSON* cJSON_DetachItemFromObject(cJSON* object, const char* string);
cJSON* cJSON_DetachItemFromObjectCaseSensitive(cJSON* object, const char* string);
void cJSON_DeleteItemFromObject(cJSON* object, const char* string);
void cJSON_DeleteItemFromObjectCaseSensitive(cJSON* object, const char* string);

/* Update array items. */
cJSON_bool cJSON_InsertItemInArray(cJSON* array, int which, cJSON* newitem); /* Shifts pre-existing items to the right. */
cJSON_bool cJSON_ReplaceItemViaPointer(cJSON* const parent, cJSON* const item, cJSON* replacement);
cJSON_bool cJSON_ReplaceItemInArray(cJSON* array, int which, cJSON* newitem);
cJSON_bool cJSON_ReplaceItemInObject(cJSON* object, const char* string, cJSON* newitem);
cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON* object, const char* string, cJSON* newitem);

/* Duplicate a cJSON item */
cJSON* cJSON_Duplicate(const cJSON* item, cJSON_bool recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
 * need to be released. With recurse!=0, it will duplicate any children connected to the item.
 * The item->next and ->prev pointers are always zero on return from Duplicate. */
 /* Recursively compare two cJSON items for equality. If either a or b is NULL or invalid, they will be considered unequal.
  * case_sensitive determines if object keys are treated case sensitive (1) or case insensitive (0) */
cJSON_bool cJSON_Compare(const cJSON* const a, const cJSON* const b, const cJSON_bool case_sensitive);

/* Minify a strings, remove blank characters(such as ' ', '\t', '\r', '\n') from strings.
 * The input pointer json cannot point to a read-only address area, such as a string constant,
 * but should point to a readable and writable address area. */
void cJSON_Minify(char* json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
cJSON* cJSON_AddNullToObject_(cJSON* const object, const char* const name);
cJSON* cJSON_AddTrueToObject_(cJSON* const object, const char* const name);
cJSON* cJSON_AddFalseToObject_(cJSON* const object, const char* const name);
cJSON* cJSON_AddBoolToObject(cJSON* const object, const char* const name, const cJSON_bool boolean);
cJSON* cJSON_AddNumberToObject_(cJSON* const object, const char* const name, const double number);
cJSON* cJSON_AddStringToObject_(cJSON* const object, const char* const name, const char* const string);
cJSON* cJSON_AddRawToObject(cJSON* const object, const char* const name, const char* const raw);
cJSON* cJSON_AddObjectToObject(cJSON* const object, const char* const name);
cJSON* cJSON_AddArrayToObject(cJSON* const object, const char* const name);

#define cJSON_SetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
double cJSON_SetNumberHelper(cJSON* object, double number);
#define cJSON_SetNumberValue(object, number) ((object != NULL) ? cJSON_SetNumberHelper(object, (double)number) : (number))
char* cJSON_SetValuestring(cJSON* object, const char* valuestring);
#define cJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)
void* cJSON_malloc(size_t size);
void cJSON_free(void* object);



namespace ylib
{
    class json
	{
	public:
		enum type
		{
			null,
			obj,
			empty,
			array,
			boolean,
			number,
			string
		};
	public:
		json();
        json(const ylib::json& value);
        json(cJSON* json,const std::string& name);



		json(const std::vector<uint32>& value);
		json(const std::vector<int32>& value);
		json(const std::vector<uint64>& value);
		json(const std::vector<int64>& value);
		json(const std::vector<const char*>& value);
        json(const std::vector<std::string>& value);
		json(const std::vector<bool>& value);
		json(const std::vector<double>& value);
		json(const std::vector<float>& value);
        json(const std::vector<ylib::json>& value);

        //json(const std::initializer_list<ylib::json>& value);
        //json(std::map<const char*, ylib::json> value);

		json(const uint32& value);
		json(const int32& value);
		json(const uint64& value);
		json(const int64& value);
        json(const std::string& value);
		json(const bool& value);
		json(const double& value);
		json(const float& value);
		json(const char* value);


		~json() ;

        bool parse(const std::string& value);
        void path(const std::string& path);
		void safe(bool safe);
		bool safe();

        static ylib::json from(const std::string& value);


		uint32 size() const;
		void resize(uint32 size);
		bool is_empty() const;
		bool is_null() const;
		bool is_obj() const;
		bool is_array() const;
		bool is_bool() const; 
		bool is_number() const;
		bool is_string() const;
        bool exist(const std::string& name) const;

        void erase(const std::string& key);
		void erase(uint32 index);
        std::vector<std::string> keys() const;

        const std::string& to_string(bool format = false) const;
        ylib::json& operator[](const std::string name);
        const ylib::json& operator[](const std::string name) const;
        ylib::json& operator[](const size_t idx);
        const ylib::json& operator[](const size_t idx) const;

		//
		void operator=(const std::vector<uint32>& value);
		void operator=(const std::vector<int32>& value);
		void operator=(const std::vector<uint64>& value);
		void operator=(const std::vector<int64>& value);
		void operator=(const std::vector<const char*>& value);
        void operator=(const std::vector<std::string>& value);
		void operator=(const std::vector<bool>& value);
		void operator=(const std::vector<double>& value);
		void operator=(const std::vector<float>& value);
        void operator=(const std::vector<ylib::json>& value);
#ifndef MSVC_2010
        void operator=(std::initializer_list<ylib::json> value);
#endif
        void operator=(std::map<const char*, ylib::json> value);



		void operator=(uint32 value);
		void operator=(int32 value);
		void operator=(uint64 value);
		void operator=(int64 value);
        void operator=(std::string value);
		void operator=(const bool value);
		void operator=(double value);
		void operator=(float value);
		void operator=(const char* value);
        void operator=(ylib::json::type value);
        void operator=(const ylib::json& value);

		void push_back(uint32 value);
		void push_back(int32 value);
		void push_back(uint64 value);
		void push_back(int64 value);
        void push_back(std::string value);
		void push_back(const bool value);
		void push_back(double value);
		void push_back(float value);
		void push_back(const char* value);
        void push_back(const ylib::json& value);
		void clear();


		template<typename T_TO>
		T_TO to(bool check = false) const
		{
			if (check)
			{
				if (is_empty())
				{

					//trw_str("JSON is empty !!! Name:"+m_name);
				}
			}
#define RETURN_T(PARAM) return *((T_TO*)&PARAM)
#define CASE_T(TYPE) std::is_same<TYPE,T_TO>::value
            if (CASE_T(std::vector<std::string>))
			{
                std::vector<std::string> result;
				if (is_array() == false)
					RETURN_T(result);
				for (uint32 i = 0;i < size();i++)
                    result.push_back(this->operator[](i).to<std::string>());
				RETURN_T(result);
			}
            else if (CASE_T(std::vector<ylib::json>))
			{
                std::vector<ylib::json> result;
				if (is_array() == false)
					RETURN_T(result);
				for (uint32 i = 0;i < size();i++)
					result.push_back(this->operator[](i));
				RETURN_T(result);
			}
			else if (CASE_T(std::vector<int>) || CASE_T(std::vector<unsigned int>) || CASE_T(std::vector<long>))
			{
				std::vector<uint32> result;
				if (is_array() == false)
					RETURN_T(result);
				for (uint32 i = 0;i < size();i++)
					result.push_back(this->operator[](i).to<int>());
				RETURN_T(result);
			}

			else if (CASE_T(std::vector<double>))
			{
				std::vector<double> result;
				if (is_array() == false)
					RETURN_T(result);
				for (uint32 i = 0;i < size();i++)
					result.push_back(this->operator[](i).to<double>());
				RETURN_T(result);
			}
			else if (CASE_T(std::vector<long long>) || CASE_T(std::vector<unsigned long long>))
			{
				std::vector<unsigned long long> result;
				if (is_array() == false)
					RETURN_T(result);
				for (uint32 i = 0;i < size();i++)
					result.push_back(this->operator[](i).to<unsigned long long>());
				RETURN_T(result);
			}
			else if (CASE_T(std::vector<float>))
			{
				std::vector<float> result;
				if (is_array() == false)
					RETURN_T(result);
				for (uint32 i = 0;i < size();i++)
					result.push_back(this->operator[](i).to<float>());
				RETURN_T(result);
			}
			else if (CASE_T(int) || CASE_T(unsigned int) || CASE_T(long))
			{
				static int result = 0;
				if (m_number)
					result = (int)this->m_value_double;
                else
                    result = ylib::stoi(m_value_string);
				RETURN_T(result);
			}
            else if (CASE_T(short) || CASE_T(unsigned short))
            {
                static short result = 0;
                if (m_number)
                    result = (short)this->m_value_double;
                else
                    result = (short)ylib::stoi(m_value_string);
                RETURN_T(result);
            }
            else if (CASE_T(std::string))
			{
				RETURN_T(this->m_value_string);
			}
			else if (CASE_T(double))
			{
				RETURN_T(this->m_value_double);
			}
			else if (CASE_T(long long) || CASE_T(unsigned long long))
			{
				static uint64 result = 0;

				if (m_number)
					result = (int)this->m_value_double;
                else
                {
                    result = ylib::stoull(m_value_string);
                }

				RETURN_T(result);
            }
			else if (CASE_T(float))
			{
			    static float result = (float)this->m_value_double;
				RETURN_T(result);
			}
			else if (CASE_T(bool))
			{
				RETURN_T(this->m_value_bool);
			}
			else
            {
				throw ylib::exception("not exception");
			}
			return T_TO();
		}

	private:
		void init(cJSON* item);
		void erase();
		cJSON* arrangement() const;
		bool is_newobj();
		void clear_type();
		bool __parse(const char* value,uint32 length);
	private:
        std::map<std::string, json*> m_objs;
		std::vector<json*> m_arrs;

        std::string m_name;
		
        std::string m_value_string;
        bool m_safe = false;
        double m_value_double = 0.0f;
        bool m_value_bool = false;

        bool m_empty = false;
        bool m_bool = false;
        bool m_array = false;
        bool m_obj = false;
        bool m_string = false;
        bool m_number = false;
        bool m_null = false;
        std::string m_jsonstring;
	};
}
