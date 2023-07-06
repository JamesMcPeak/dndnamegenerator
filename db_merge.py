#db_merge.py

for heritage in heritages:
    for table in medtables:
        tableheritage = table.split("_")[0].replace("medieval","")
        if tableheritage == heritage:
            tabletype = table.split("_")[1]
            try:
                tablegender = table.split("_")[2]
            except:
                tablegender = "any"
            cur.execute("SELECT * FROM " + table)
            rows = cur.fetchall()
            for row in rows:
                cur.execute("SELECT EXISTS(SELECT 1 FROM " + heritage + " WHERE name=\"" + row[0] + "\");")
                if (cur.fetchone()):
                    cur.execute("UPDATE " + heritage + " SET medieval = 'true' WHERE name = '" + row[0] + "';")
                else:
                    cur.execute("INSERT INTO " + heritage + " (name, type, gender, length, medieval, aristocratic) VALUES ('" + row[0] + "', '" + tabletype + "', '" + tablegender + "', " + str(row[1]) + ", 'true', 'false');")

for n in eanl:
    cur.execute("SELECT EXISTS(SELECT 1 FROM english WHERE name=\"" + n + "\");")
    if (cur.fetchone()):
        cur.execute("UPDATE english SET aristocratic = 'true' WHERE name = '" + n + "';")
    else:
        print(n + " was not present in the database.")
