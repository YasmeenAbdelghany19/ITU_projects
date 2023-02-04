from flask import Flask
import mysql.connector as mysql

app = Flask(__name__)

from app import views, server