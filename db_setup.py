#db_setup.py
import csv, sqlite3

con = sqlite3.connect("C:/Users/James/Documents/projects/dndnamegenerator/dndnamegenerator/myDb.db")

cur = con.cursor()

while True:
    tablename = input("Table Name ==> ")
    columndata = input ("Column Data ==> ")
    columntype = input("Column Type ==> ")
    columngender = input("Column Gender ==> ")
    names = columndata.split(",")
    for name in names:
        cur.execute("INSERT INTO " + tablename + "(name, type, gender, length, medieval, aristocratic) VALUES ('" + name.strip() + "', '" + columntype + "', '" + columngender + "', " + str(len(name.strip()))+", 'false', 'false');")
    con.commit()
    print("Successfully updated table " + tablename)

con.close()
