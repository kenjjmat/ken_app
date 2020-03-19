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

    // displaying error messsage 
    std::string sqlite_error(int error_code) {
        std::string error = sqlite3_errstr(error_code);
        if (error == "not an error") error.clear();
        if (error.length() > 0) error[0] = toupper(error[0]);
        return error;
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
    int error_code = 0;
    if (password.empty()) {
        error_code = sqlite3_open_v2(file_name.c_str(), &d_.db,
            SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX, NULL);
        error_code = sqlite3_exec(d_.db, "SELECT count(*) FROM sqlite_master;", NULL, NULL, NULL);
    }
    else
    {
        error_code = sqlite3_open_v2(file_name.c_str(), &d_.db,
            SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX, NULL);

        sqlite3_stmt* stm = nullptr;
        const char* pzTail = nullptr;

        //key the database 
        auto pragma_string = "PRAGMA key = '" + password + "';";
        sqlite3_prepare(d_.db, pragma_string.c_str(), -1, &stm, &pzTail);
        sqlite3_step(stm);
        sqlite3_finalize(stm);

        // test if key is correct 
        error_code = sqlite3_exec(d_.db, "SELECT count(*) FROM sqlite_master;", NULL, NULL, NULL);


    }
    if (error_code != SQLITE_OK) {
        //an error occured 
        error = d_.sqlite_error(error_code);

        // close the database 
        sqlite3_close(d_.db);
        d_.db = nullptr;
        return false;

    }

    // create tables 

    table table;

    // users 
    if (d_.sqlite_query("CREATE TABLE Users ("
        "Username TEXT NOT NULL , "
        "Password TEXT NOT NULL,"
        "PRIMARY KEY (Username)"
        ");", table, error)) {
        if (error.find("already exists") == std::string::npos) return false;
    }

    d_.connected_ = true;
    return true;
}

bool ken_app_db::new_user(const user_credentials& user, const std::string& password, std::string& error)
{
    if (d_.connected_) {
        error = "Not connected to database"; 
        return false;
    }
    table table;
    auto new_user_ = d_.sqlite_query("INSERT INTO Users VALUES('"
        + user.username + "' , '" + password + "');", table, error);

    return new_user_;
}

bool ken_app_db::get_users(std::vector<user_credentials>& users, std::string& error)
{
    users.clear();
    if (!d_.connected_) {
        error = "Not connnected to database"; return false;
    }

    table table;
    if (!d_.sqlite_query("SELECT * FROM Users;", table, error)) return false;

    //reading the database
    for (const auto& it : table) {
        user_credentials user;
        user.username = it.at("Username");   
    }
    return false;
}

bool ken_app_db::get_user(const std::string& username, const std::string& password, user_credentials& user, std::string& error)
{
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    table table;
    if (!d_.sqlite_query("SELECT * FROM Users WHERE Username = '"
        + username + "' AND Password = '" + password + "';", table, error))
        return false;

    if (table.empty()) {
        error = "Incorect Credentials (Username or Password)";
        return false;
        }

    user.username = table.at(0).at("Username");
    return true;
}

bool ken_app_db::get_user(const std::string& username, user_credentials& user, std::string& error)
{

    if (!d_.connected_) {
        error = "Not connected to the database"; return false;
    }

    table table;
    if (!d_.sqlite_query("SELECT * FROM Users WHERE Username ='"
        + username + "';", table, error)) return false;

    if (table.empty()) {
        error = "Incorrect Credentials(Username or Password)";
        return false;
    }

    user.username = table.at(0).at("Username");


    return true;
}



