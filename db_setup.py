#db_setup.py
import csv, sqlite3

con = sqlite3.connect("C:/Users/James/Documents/projects/dndnamegenerator/dndnamegenerator/myDb.db")

cur = con.cursor()

while True:
    tablename = input("Table Name ==> ")
    tabledata = input("Table Data ==> ")
    data = tabledata.split(",")
    cur.execute("CREATE TABLE IF NOT EXISTS " + tablename + "(name TEXT, length INT);")
    for datum in data:
        cur.execute("INSERT INTO " + tablename + "(name, length) VALUES ('" + datum.strip() + "', " + str(len(datum.strip()))+");")
    con.commit()
    print("Successfully created table " + tablename)

con.close()
