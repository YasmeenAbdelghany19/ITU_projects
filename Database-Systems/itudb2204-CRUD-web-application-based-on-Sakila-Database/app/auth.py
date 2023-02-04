from flask import current_app
from flask_login import UserMixin
from app import app


class User(UserMixin):
    def __init__(self, username, password):
        self.username = username
        self.password = password