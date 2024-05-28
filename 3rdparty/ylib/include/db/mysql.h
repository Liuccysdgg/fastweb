#pragma once

#include <vector>
#include <string>
#include <list>
#ifndef _WIN32 
#include <stddef.h>
#endif
#include "base/define.h"
#include "base/error.h"
#include "util/json.h"
#include "util/pool.hpp"

namespace ylib::mysql
{
    class pool;
    class result;
    class conn;
    class prepare_statement;
    struct field {
        uint32 index = 0;
        std::string name;
        std::string type_name;
    };
    struct mysql_conn_info
    {
        mysql_conn_info() {
            port = 0;
        }
        std::string ipaddress;
        std::string username;
        std::string password;
        std::string database;
        std::string charset;
        uint32 port;
    };
    class result :public ylib::error_base
    {
    public:
        result(void* handle);
        ~result();
        // 列名
        std::string field_name(uint32 index);
        // 列类型
        std::string field_type(uint32 index);
        std::string field_type(const std::string& name);
        // 列数量 
        uint32 field_count();
        // 行数量
        size_t row_count();
        // 下一行
        bool next();
        int32 get_int32(uint32 index);
        int32 get_int32(const std::string& name);

        uint32 get_uint32(uint32 index);
        uint32 get_uint32(const std::string& name);

        int64 get_int64(uint32 index);
        int64 get_int64(const std::string& name);

        uint64 get_uint64(uint32 index);
        uint64 get_uint64(const std::string& name);

        std::string get_string(uint32 index);
        std::string get_string(const std::string& name);

        bool get_boolean(uint32 index);
        bool get_boolean(const std::string& name);

        double get_double(uint32 index);
        double get_double(const std::string& name);

        ylib::json to_json();

    private:
        void* m_handle = nullptr;
        uint32 m_field_count = 0;
        uint32 m_row_count = 0;

        std::vector<ylib::mysql::field> m_fields;
    };

    class prepare_statement :public ylib::error_base {
    public:
        prepare_statement();
        ~prepare_statement();
        void set_bigint(uint32 index, const std::string& value);
        void set_boolean(uint32 index, bool value);
        void set_datetime(uint32 index, const std::string& value);
        void set_double(uint32 index, double value);
        void set_int32(uint32 index, int32 value);
        void set_uint32(uint32 index, uint32 value);
        void set_int64(uint32 index, int64 value);
        void set_uint64(uint32 index, uint64 value);
        void set_null(uint32 index);
        void set_string(uint32 index, const std::string& value);
        void set_blob(uint32 index, const ylib::buffer& value);
        void clear();
        uint64 update();
        ylib::mysql::result* query();

        friend class ylib::mysql::conn;
    private:
        ylib::mysql::result* m_result = nullptr;
        void* m_handle = nullptr;
    };
    class conn :public ylib::example<ylib::mysql::mysql_conn_info>, public ylib::error_base
    {
    public:
        conn();
        ~conn();
        virtual EXAMPLE_START_RESULT start(const ylib::mysql::mysql_conn_info& info) override;
        virtual void close() override;


        virtual void recover() override;
        virtual void task_out() override;


        void clear();
        ylib::mysql::prepare_statement* setsql(const std::string& sql);
        uint64 insert_id();

        void begin(bool autocommit = false);
        void commit();
        void rollback();

        void setDatabase(const std::string& name);

        friend class ylib::mysql::pool;
    private:
        void* m_handle = nullptr;
        ylib::mysql::mysql_conn_info m_info;
        class prepare_statement* m_ppst = nullptr;
        // 事务状态 0=未开启 1=已开启 2=执行完毕
        int m_sw = 0;
    };


    class pool : public ylib::pool<ylib::mysql::conn, ylib::mysql::mysql_conn_info>
    {
    public:
        pool()
        {

        }
        ~pool()
        {

        }
    };



    /// <summary>
    /// 协程调用
    /// 执行期间会让出协程，并等待再次调度
    /// </summary>
    /// <param name="ppst"></param>
    /// <returns></returns>
    std::tuple<bool, std::string> co_query(ylib::mysql::prepare_statement* ppst);
}
