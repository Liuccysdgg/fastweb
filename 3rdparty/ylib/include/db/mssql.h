#pragma once
#ifdef _WIN32
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

namespace ylib::mssql
{
    class pool;
    class result;
    class conn;
    class prepare_statement;
    struct mssql_conn_info
    {
        mssql_conn_info() {
        }
        std::string odbcname;
        std::string username;
        std::string password;
    };
    class result :public ylib::error_base
    {
    public:
        result();
        ~result();
        // 列数量
        uint32 field_count();
        // 行数量
        size_t row_count();
        // 取文本值
        std::string get(uint32 field);
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

        friend class ylib::mssql::prepare_statement;
    private:
        void* m_handle = nullptr;
        uint32 m_field_count = 0;
        uint32 m_row_count = 0;

        std::map<std::string, short> m_field_name_index;
    };

    class prepare_statement :public ylib::error_base {
    public:
        prepare_statement();
        ~prepare_statement();
        void set_boolean(uint32 index, bool value);
        void set_datetime(uint32 index, const std::string& value);
        void set_double(uint32 index, double value);
        void set_int32(uint32 index, int32 value);
        void set_uint32(uint32 index, uint32 value);
        void set_int64(uint32 index, int64 value);
        void set_uint64(uint32 index, uint64 value);
        void set_null(uint32 index);
        void set_string(uint32 index, const std::string& value);
        void clear();
        uint64 update();
        ylib::mssql::result* query();

        friend class ylib::mssql::conn;
    private:
        ylib::mssql::result* m_result = nullptr;
        void* m_handle = nullptr;
    };
    class conn :public ylib::example<ylib::mssql::mssql_conn_info>, public ylib::error_base
    {
    public:
        conn();
        ~conn();
        virtual EXAMPLE_START_RESULT start(const ylib::mssql::mssql_conn_info& info) override;
        virtual void close() override;


        virtual void recover() override;
        virtual void task_out() override;


        void clear();
        ylib::mssql::prepare_statement* setsql(const std::string& sql);
        void begin();
        void commit();
        void rollback();
        friend class ylib::mssql::pool;
    private:
        void* m_handle = nullptr;
        ylib::mssql::mssql_conn_info m_info;
        class prepare_statement* m_ppst = nullptr;
        int m_sw = 0;
    };


    class pool : public ylib::pool<ylib::mssql::conn, ylib::mssql::mssql_conn_info>
    {
    public:
        pool();
        ~pool();
    };
}
#endif