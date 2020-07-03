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
                    if (sqlite3_step(statement) == SQLITE_ROW) {
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

    // users table
    if (!d_.sqlite_query("CREATE TABLE Users ("
        "Username TEXT NOT NULL , "
        "Password TEXT NOT NULL,"
        "PRIMARY KEY (Username)"
        ");", table, error)) {
        if (error.find("already exists") == std::string::npos)
            return false;
    }

    //stock details table
    if (!d_.sqlite_query("CREATE TABLE Stock ("
        "ID TEXT NOT NULL, "
        "Name TEXT NOT NULL, "
        "Description TEXT NOT NULL, "
        "Quantity INTEGER NOT NULL , "
        "PRIMARY KEY (ID)"
        ");",
        table, error)) {
        if (error.find("already exists") == std::string::npos) return false;
    }

   
    // sales details table
    if (!d_.sqlite_query("CREATE TABLE Sales ("
        "ID TEXT NOT NULL, "
        "Item_Name TEXT NOT NULL, "
        "Quantity INTEGER NOT NULL, "
        "Unit_price INTEGER NOT NULL , "
        "Cost INTERGER NOT NULL , "
        "PRIMARY KEY (ID)"
        ");",
        table, error)) {
        if (error.find("already exists") == std::string::npos) return false;
    }

    // appointment details Table
    if (!d_.sqlite_query("CREATE TABLE Appointment ("
        "ID INTEGER NOT NULL, "
        "Time TIME NOT NULL, "
        "Date DATE NOT NULL, "
        "Name TEXT NOT NULL, "
        "Surname TEXT NOT NULL , "
        "Description TEXT NOT NULL , "
        "PRIMARY KEY (ID)"
        ");",
        table, error)) {
        if (error.find("already exists") == std::string::npos) return false;
    }

    d_.connected_ = true;
    return true;
}



bool ken_app_db::new_user(const user_credentials& user, const std::string& password, std::string& error)
{
    if (!d_.connected_) {
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
        users.push_back(user);
    }

    return true;
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

// this code is for adding people ino the actual database;
bool ken_app_db::new_stock(const stock_details& stock_info, std::string& error)
{
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
     }

    table table;
    if (!d_.sqlite_query("INSERT INTO Stock VALUES('"
        + stock_info.id + "' , '" + stock_info.name + "' , '" + stock_info.description + "' , '" + stock_info.quantity +
        "');", table, error)) {
        return false;
    }
    return true;
}

// now we are getting data from te database and storing it into the table which is a map of type vector
bool ken_app_db::get_stock(const std::string& stock_id, stock_details& stock, std::string& error)
{
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    // validating whether the user has entered the primary key or not
    if (stock_id.empty()) {
        error = "Stock ID not specified";
        return false;
    }

    // reading the database 
    table table;
    if (!d_.sqlite_query("SELECT * FROM Stock WHERE ID = '"
        + stock_id + "';", table, error)) return false;

    // now assigning values from the database table to the struct stock details
    if (!table.empty()) {
        stock_details stock_;
        stock_.id = table.at(0).at("ID");
        stock_.name = table.at(0).at("Name");
        stock_.description = table.at(0).at("Description");
        stock_.quantity = table.at(0).at("Quantity");

        stock = stock_;
    }


    return true;
}

// now taking the values from the table and inserting then into a vector stock
bool ken_app_db::get_stock_all(std::vector<stock_details>& stock, std::string& error)
{
    // clear the vector first
    stock.clear();
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    // read the database 
    table table;
    if (!d_.sqlite_query("SELECT * FROM Stock;", table, error)) return false;

    if (!table.empty()) {
        
     // looping through the table and inserting into the vector stock
        for (const auto& row : table) {
            stock_details stock_;
            stock_.id = row.at("ID");
            stock_.name = row.at("Name");
            stock_.description = row.at("Description");
            stock_.quantity = row.at("Quantity");

            stock.push_back(stock_);
        }
    }
    return true;
}

// adding sales
bool ken_app_db::new_sales(const sales_details& sales_info, std::string& error)
{
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    table table;
    if (!d_.sqlite_query("INSERT INTO Sales VALUES('" + sales_info.id +
        "', '" + sales_info.item_name + "' , '" + sales_info.quantity + "' , '" + sales_info.Unit_price + "' , '" + sales_info.Cost + "');", table, error)) {
        return false;
    }
    return true;
}


// get sales
bool ken_app_db::get_sales(const std::string& sales_id, sales_details& sales, std::string& error)
{
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    // validating whether the user has entered the primary key or not
    if (sales_id.empty()) {
        error = "Sales ID not specified";
        return false;
    }

    // reading the database 
    table table;
    if (!d_.sqlite_query("SELECT * FROM Sales WHERE ID = '"
        + sales_id + "';", table, error)) return false;

    // now assigning values from the database table to the struct stock details
    if (!table.empty()) {
        sales_details sales_;
        sales_.id = table.at(0).at("ID");
        sales_.item_name = table.at(0).at("Item_Name");
        sales_.quantity = table.at(0).at("Quantity");
        sales_.Unit_price = table.at(0).at("Unit_price");
        sales_.Cost = table.at(0).at("Cost");

        sales = sales_;
    }


    return true;
}

// retrieve all the sales in the database
bool ken_app_db::get_sales_all(std::vector<sales_details>& sales, std::string& error)
{
    // clear the vector first
    sales.clear();
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    // read the database 
    table table;
    if (!d_.sqlite_query("SELECT * FROM Sales;", table, error)) return false;

    if (!table.empty()) {

        // looping through the table and inserting into the vector stock
        for (const auto& row : table) {
            sales_details sales_;
            sales_.id = row.at("ID");
            sales_.item_name = row.at("Item_Name");
            sales_.quantity = row.at("Quantity");
            sales_.Unit_price = row.at("Unit_price");
            sales_.Cost = row.at("Cost");

            sales.push_back(sales_);
        }
    }
    return true;
}

// appointments

bool ken_app_db::new_appointment(const appointments_details& appointment_info, std::string& error)
{
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    table table;
    if (!d_.sqlite_query("INSERT INTO Appointment VALUES('"
        +  appointment_info.time + "','" + appointment_info.id +"','"+ appointment_info.date +"' , '" + appointment_info.name + "' , '" + appointment_info.surname + "' , '" + appointment_info.description +
        "');", table, error)) {
        return false;
    }
	return true;
}

// appointments
bool ken_app_db::get_appointment(const std::string& appointment_id, appointments_details& appointment, std::string& error)
{

    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

     //validating whether the user has entered the primary key or not
    if (appointment_id.empty()) {
        error = "Appointment ID not specified";
        return false;
    }

    // reading the database 
    table table;
    if (!d_.sqlite_query("SELECT * FROM Appointment WHERE  ID = '"
        + appointment_id + "';", table, error)) return false;

    // now assigning values from the database table to the struct stock details
    if (!table.empty()) {
        appointments_details appointment_;
        appointment_.id = table.at(0).at("ID");
        appointment_.time = table.at(0).at("Time");
        appointment_.date = table.at(0).at("Date");
        appointment_.name = table.at(0).at("Name");
        appointment_.surname = table.at(0).at("Surname");
        appointment_.description = table.at(0).at("Description");

        appointment= appointment_;
    }
    return true;
}

// appointments

bool ken_app_db::get_appointments(std::vector<appointments_details>& appointment, std::string& error)
{
    // clear the vector first
    appointment.clear();
    if (!d_.connected_) {
        error = "Not connected to database"; return false;
    }

    // read the database 
    table table;
    if (!d_.sqlite_query("SELECT * FROM Appointment;", table, error)) return false;

    if (!table.empty()) {

        // looping through the table and inserting into the vector stock
        for (const auto& row : table) {
            appointments_details appointment_;
            appointment_.id = row.at("ID");
            appointment_.name = row.at("Name");
            appointment_.time = row.at("Time");
            appointment_.date = row.at("Date");
            appointment_.surname = row.at("Surname");
            appointment_.description = row.at("Description");

            appointment.push_back(appointment_);
        }
    }
    return true;
}




