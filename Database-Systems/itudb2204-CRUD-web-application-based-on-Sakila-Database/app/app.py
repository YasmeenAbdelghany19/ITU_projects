from flask import Flask, render_template, url_for, request, redirect, jsonify
from flask_mysqldb import MySQL, MySQLdb
from flask import flash
from datetime import datetime

app = Flask(__name__)

app.secret_key = 'your secret key'

app.config['MYSQL_HOST'] = "localhost"
app.config['MYSQL_USER'] = "root"
app.config['MYSQL_PASSWORD'] = "Fur19Kan98"
app.config['MYSQL_DB'] = "sakila"
app.config["MYSQL_CURSORCLASS"] = "DictCursor"

mysql = MySQL(app)
