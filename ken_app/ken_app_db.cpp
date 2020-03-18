#include "ken_app_db.h"
#include <iostream>
#include <string>
#include <map>
#include "state.h"

using table = std::vector<std::map<std::string, std::string>>;

class ken_app_db::ken_app_db_impl {
    friend ken_app_db;

    sqlite3* db;
    bool connected_;
    std::string file_name;
    std::string password;

public:
    ken_app_db_impl() :
        connected_(false), db(nullptr) {}

    ~ken_app_db_impl() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }
          
    // displaying the error in the database

    std::string sqlite_error() {
        if (db) {
            std::string error = sqlite3_errmsg(db);
            if (error == "not an error") error.clear();
            if (error.length() > 0) error[0] = toupper(error[0]);
            return error;
        }
        else {
            return "Database not open";
        }
    }
    //sqlite query

    bool sqlite_query(const std::string& query, table& table, std::string& error) {
        table.clear();

        if (db) {
            sqlite3_stmt* statement = nullptr;

            if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
                const int columns = sqlite3_column_count(statement);

                while (true) {
                    if (sqlite3_step(statement) == SQLITE_OK) {
                        std::map<std::string, std::string > values;

                        for (int column = 0; column < columns; column++) {
                            std::string column_name, value;

                            // get column name
                            char* ccColumn = (char*)sqlite3_column_name(statement, column);
                            if (ccColumn) {
                                column_name = ccColumn;

                                // get data
                                char* ccData = (char*)sqlite3_column_text(statement, column);

                                if (ccData)
                                    value = ccData;

                                values.insert(std::make_pair(column_name, value));

                            }


                        }
                        table.push_back(values);
                    }
                    else
                        break;
                }
                sqlite3_finalize(statement);
            }
            else {
                error = sqlite_error();
                return false;
            }
            return true;
        }
        else {
            error = "Database not open";
            return false;
        }
    }
};

ken_app_db::ken_app_db():
    d_ (*(new ken_app_db::ken_app_db_impl())){
}

ken_app_db::~ken_app_db(){
    delete& d_;
}

bool ken_app_db::connect(const std::string& file_name, const std::string& password, std::string& error){

    if (d_.connected_) return true;

    if (password.empty()) {}

    return false;
}

bool ken_app_db::new_user(const users& user, const std::string& password, std::string& error)
{
    return false;
}

bool ken_app_db::get_users(std::vector<users>& users, std::string& error)
{
    return false;
}

bool ken_app_db::get_user(const std::string& username, const std::string& password, users& user, std::string& error)
{
    return false;
}

bool ken_app_db::get_user(const std::string& username, users& user, std::string& error)
{
    return false;
}
