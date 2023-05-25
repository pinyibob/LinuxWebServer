import sys
import os
import datetime
import mysql.connector

def connect_to_database(user, password, host, port, database):
    connection = mysql.connector.connect(
        user=user,
        password=password,
        host=host,
        port=port,
        database=database
    )
    return connection

def get_file_metadata(file_path):
    metadata = os.stat(file_path)
    create_date = metadata.st_ctime
    create_date_str = datetime.datetime.fromtimestamp(create_date).strftime('%Y-%m-%d %H:%M:%S')
    last_edit_date = metadata.st_mtime
    last_edit_date_str = datetime.datetime.fromtimestamp(last_edit_date).strftime('%Y-%m-%d %H:%M:%S')
    return (os.path.basename(file_path), create_date_str, last_edit_date_str)

def insert_article_info(connection, filename, create_date, last_edit_date, title, id, tag):
    cursor = connection.cursor()
    query = "INSERT INTO article_info (article_id, file_name, create_date, last_edit_date, title, tag) VALUES (%s, %s, %s, %s, %s, %s)"
    values = (id, filename, create_date, last_edit_date, title, tag)
    cursor.execute(query, values)
    connection.commit()
    cursor.close()

if __name__ == "__main__":
    if len(sys.argv) < 5:
        print("Please provide path to .md file and title string")
        sys.exit(1)
    file_path = sys.argv[1]
    title = sys.argv[2]
    article_id = sys.argv[3]
    tag = sys.argv[4]

    connection = connect_to_database("root", "123456", "localhost", "3306", "pinyibob_blog")
    filename, create_date, last_edit_date = get_file_metadata(file_path)
    insert_article_info(connection, filename, create_date, last_edit_date, title, article_id, tag)
    connection.close()