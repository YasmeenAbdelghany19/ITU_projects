from app import app

from flask import render_template, request, redirect, url_for, flash, jsonify

import mysql.connector as mysql
import MySQLdb.cursors


app.secret_key = "top secret"




mydb = MySQLdb.connect(
    host="localhost",
    user="root",
    database="sakila",
    password="Fur19Kan98",
    cursorclass=MySQLdb.cursors.DictCursor
)

#### INSERTING INTO USER TABLE ####
def insert_user(username, password):
    try:
        cur = mydb.cursor()
        statement = """ insert into users
                        (username, passcode)
                        values (%s, %s)
                        """
        cur.execute(statement, (username, password))
        mydb.commit()
        flash("Account created!", category="success")
    except:
        flash("Data isn't inserted into user table! Please try again!!", category="error")

#### FETCHING USER ####
def check_user(username):
    try:
        cur = mydb.cursor()
        statement = """ select * from users 
                        where username = %s
                        """
        cur.execute(statement, (username, ))
        user = cur.fetchone()
        cur.close()
        return user
    except:
        flash("Username not found! Sign up or retype your username.", category="error")
         
#### INSERTING INTO FILM TABLE ####
def insert_film(title, description, release_year, language_id, rental_duration,
                rental_rate, length, replacement_cost, rating):
    try:
        cur = mydb.cursor()
        statement = """ insert into film
                        (title, description, release_year, language_id, rental_duration,
                        rental_rate, length, replacement_cost, rating)
                        values (%s, %s, %s, %s, %s, %s, %s, %s, %s)
                        """
        cur.execute(statement, (title, description, release_year, language_id, rental_duration,
                                rental_rate, length, replacement_cost, rating))
        mydb.commit()
        flash("Data inserted into film table successfully!", category="success")
    except:
        flash("Data isn't inserted into film table! Please try again!!", category="error")

#### INSERTING INTO FILM_CATEGORY TABLE ####
def insert_film_cat(film_id, cat_id):
    try:
        cur = mydb.cursor()
        
        statement = """ insert into film_category (film_id, category_id) values (%s, %s) """
        cur.execute(statement, (film_id['film_id'], cat_id))
        mydb.commit()
        flash("Data inserted into film_category table successfully!", category="success")
    except:
        flash("Data isn't inserted into film_category table! Please try again!!", category="error")

##### FETCHING FILM ID ####
def fetch_id(title,cat_id):
    try:
        cur = mydb.cursor()
        cur.execute(
            """SELECT film_id FROM film WHERE title = %s """, (title,))
        film_id = cur.fetchone()
        insert_film_cat(film_id, cat_id)
    except:
        flash("No such film exists. Can't add to film_category table", category="error")

#### INNER JOINING FILM,FILM_CATEGORY AND LANGUAGE TABLES ####
def fetch_film():
    try:
        cursor = mydb.cursor()
        cursor.execute("""SELECT
                                f.film_id, f.title, c.name, f.description, f.release_year,
                                l.name as language, f.rental_duration, f.rental_rate, f.length,
                                f.replacement_cost, f.rating
                            FROM
                                film f
                            JOIN
                                language l ON f.language_id = l.language_id
                            JOIN
                                film_category fc on f.film_id = fc.film_id
                            JOIN
                                category c ON fc.category_id = c.category_id
                            order by f.film_id;""")

        records = cursor.fetchall()
        return records
    except:
        flash("Couldn't fetch data from database!", category="error")
    
#### FETCHING LANGUAGE ####
def fetch_language():
    try:
        cursor = mydb.cursor()
        cursor.execute("""SELECT
                                l.name
                            FROM
                                language l;""")
        records = cursor.fetchall()
        languageArr=[]
        for row in records:
            languageArr.append(row['name'])
        
        return languageArr
    except:
        flash("Couldn't fetch data from database!", category="error")
   
#### FETCHING CATEGORY ####
def fetch_category():
    try:
        cursor = mydb.cursor()
        cursor.execute("""SELECT
                                name
                            FROM
                                category;""")
        records = cursor.fetchall()
        catArr=[]
        for row in records:
            catArr.append(row['name'])
        
        return catArr
    except:
        flash("Couldn't fetch data from database!", category="error")
    
#### UPDATING FILM TABLE ####
def update_film(title, description, year, lang_id,
                    rent_dur, rent_rate, length, cost, rating, film_id):
    try:
        cur = mydb.cursor()
        cur.execute(
            """UPDATE film
            SET title = %s, description = %s, release_year = %s, language_id = %s,
            rental_duration = %s, rental_rate = %s, length = %s, replacement_cost = %s,
            rating = %s
            WHERE film_id = %s """, (title, description, year, lang_id,
                                        rent_dur, rent_rate, length, cost, rating, film_id))
        mydb.commit()
        flash("Data updated in film table successfully!", category="success")
    except:
        flash("Data couldn't be updated in film table! PLease try again.", category="error")

#### UPDATING FILM_CATEGORY TABLE ####
def update_film_category(cat_id, film_id):
    try:
        cur = mydb.cursor()
        cur.execute(
            """UPDATE film_category
            SET category_id = %s
            WHERE film_id = %s """, (cat_id, film_id))
        mydb.commit()
        flash("Data updated in film_category table successfully!", category="success")
    except:
        flash("Data couldn't be updated in film_category table! PLease try again.", category="error")

#### DELETE FROM FILM TABLE ####
def delete_film(film_id):
    try:
        cur = mydb.cursor()
        cur.execute(
            """DELETE FROM film
            WHERE film_id = %s """, (film_id, ))
        mydb.commit()
        flash("The film was deleted successfully!", category="success")
    except:
        flash("The film couldn't be deleted! PLease try again.", category="error")

#### INSERT INTO LANGUAGE TABLE ####
def insert_language(language):
    try:
        cur = mydb.cursor()
        statement = """ insert into language (name) values (%s) """
        cur.execute(statement, (language, ))
        mydb.commit()
        flash("New language added successfully", category="success")
    except:
        flash("Couldn't add a new language! Please try agian.", category="error")

#### INSERT INTO CATEGORY TABLE ####
def insert_category(category):
    if request.method == "POST":
        try:
            cur = mydb.cursor()
            statement = """ insert into category (name) values (%s) """
            cur.execute(statement, (category, ))
            mydb.commit()
            flash("New category added successfully", category="success")
        except:
            flash("Couldn't add a new category! Please try agian.", category="error")

############################## staff page ######################333


#### INNER JOINING STAFF,STORE,ADDRESS,CITY COUNTRY TABLES TO SHOW IN STAFF PAGE ####
def fetch_staff():
    try:
        cur = mydb.cursor()
        cur.execute("""Select s.staff_id,s.first_name,s.last_name,
                            s.username,s.email,co.country,ci.city,a.address
                            as Store_Address,a2.address as Home_Address from sakila.staff s 
                            INNER JOIN sakila.store as st ON st.store_id = s.store_id 
                            INNER JOIN sakila.address as a ON st.address_id = a.address_id 
                            INNER JOIN sakila.city as ci ON a.city_id = ci.city_id 
                            INNER JOIN sakila.country as co ON co.country_id = ci.country_id 
                            INNER JOIN sakila.address as a2 ON a2.address_id = s.address_id order by s.staff_id""")
        control = cur.fetchall()
    except:
        flash("Couldn't fetch data from database!", category="error")
    return control

#### FETCHING COUNTRY ####
def fetch_country():
    try:
        cur2 = mydb.cursor()
        cur2.execute("""Select co.country_id,co.country from sakila.country co
                                    INNER JOIN sakila.city as ci on ci.country_id = co.country_id 
                                    INNER JOIN sakila.address as a ON a.city_id = ci.city_id 
                                    INNER JOIN sakila.store as st on a.address_id = st.address_id""")
        countries = cur2.fetchall()
    except:
        flash("Couldn't fetch data from database!", category="error")
    return countries

#### INSERTING INTO STAFF TABLE ####
def insert_staff(first_name, last_name, addressId, email, storeId, username, password):
        try:
            cur = mydb.cursor()
            cur.execute("""INSERT INTO sakila.staff 
                    (first_name,last_name,address_id,email,store_id,active,username,password) 
                    values (%s,%s,%s,%s,%s,1,%s,%s)""",
                    (first_name, last_name, addressId, email, storeId, username, password))
            mydb.commit()
            cur.close()
            flash("Data inserted into staff table successfully!", category="success")
        except:
            flash("Data not inserted into staff table successfully!", category="wrong")

#### INSERTING NEW ADDRESS ####
def insert_address(id,name):
    try:
        cur = mydb.cursor()
        cur.execute("Insert into sakila.address (address,city_id,district,phone) values (%s,%s,%s,%s)",(name,id,"",""))
        mydb.commit()
        cur.close()
        cur2 = mydb.cursor()
        cur2.execute("Select address_id from sakila.address where address = %s" , (name,))
        address = cur2.fetchone()
        flash("Data inserted into address table successfully!", category="success")
        return address
    except:
        flash("Data is not inserted into address table successfully!", category="error")

#### DELETE FROM STAFF TABLE ####
def delete_staff(id):
    try:
        if manager_control(id):
            flash("MANAGERS CAN'T BE DELETED")
        else:
            cur = mydb.cursor()
            cur.execute("DELETE FROM sakila.staff where staff_id = %s" % id)
            mydb.commit()
            cur.close()
            flash("Data deleted from staff table successfully!", category="success")
        
    except:
        flash("Couldn't delete from staff! Please try again", category="error")

#### UPDATING STAFF TABLE ####
def update_staff(fname, lname, email, stAdd, username, hAdd, int_id):
    try:
        if manager_control(int_id):
            flash("MANAGERS CAN'T BE EDITED")
        else:
             cur = mydb.cursor()
             cur.execute("Update sakila.staff SET first_name = %s ,last_name = %s,email=%s,store_id=%s,username=%s,address_id=%s where staff_id = %s",
                     (fname, lname, email, stAdd, username, hAdd, int_id))
             mydb.commit()
             cur.close()
             flash("Staff table updated successfully!", category="success")
    except:
        flash("Couldn't update staff table! Please try again", category="error")

#### CONTROLLING WHETHER THE STAFF IS MANAGER ####
def manager_control(id):
    cur=mydb.cursor()
    cur.execute("Select st.manager_staff_id from sakila.store st where manager_staff_id = %s" % id)
    isManager = cur.fetchone()
    if type(isManager) == dict:
        
        return True
    else:
        return False



#### FETCHING CITY FOR DYNAMIC SHOW IN STAFF.JAVASCRIPT ####        
def fetch_city(id):
    try:
        cur = mydb.cursor()
        result = cur.execute(
            """Select ci.city_id,ci.city FROM sakila.city ci 
            INNER JOIN sakila.address as a on a.city_id = ci.city_id 
            INNER JOIN sakila.store as st on st.address_id = a.address_id
            where ci.country_id = %s""" % id)
        city = cur.fetchall()
    except:
        flash("Couldn't fetch data from city table! Please try agian.", category="error")
    return city

#### FETCHING STORE ID AND ADDRESS FOR DYNAMIC SHOW IN STAFF.JAVASCRIPT ####
def fetch_address(id):
    try:
        cur = mydb.cursor()
        result = cur.execute(
            """Select st.store_id,a.address from sakila.address a 
            INNER JOIN sakila.store as st on st.address_id = a.address_id 
            WHERE a.city_id =%s""" % id)
        store_Address = cur.fetchall()
    except:
        flash("Couldn't fetch data from address table! Please try agian.", category="error")
    return store_Address

####FETCHING STAFF ADDRESS FOR DYNAMIC SHOWING #####
def fetch_staff_address(id):
    try:
        cur=mydb.cursor()
        result = cur.execute("Select address_id,address from sakila.address where city_id = %s" % id)
        store_Address = cur.fetchall()
        return store_Address
    except:
        flash("Couldn't fetch data from address table! Please try agian.", category="error")
####FETCHING STAFF HOME ADDRESS FOR DYNAMIC SHOWING #####        
def fetch_Home_Address(id):
    try:
        cur = mydb.cursor()
        result = cur.execute("Select a.address_id,a.address from sakila.address a INNER JOIN sakila.city as ci on ci.city_id = a.city_id where ci.city_id=%s" % id)
        home_Address = cur.fetchall()
        return home_Address
    except:
        flash("Couldn't fetch data from address and city tables! Please try again.", category="error")

