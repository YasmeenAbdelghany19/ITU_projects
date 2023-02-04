from flask import Flask, abort, render_template, request, redirect, url_for, jsonify, flash, session
from app import app, views, auth
from werkzeug.security import generate_password_hash, check_password_hash
from datetime import timedelta

app.permanent_session_lifetime=timedelta(minutes=5)
#### LOGIN PAGE ####
@app.route('/login', methods=["POST", "GET"])
def login():
    if request.method == "POST":
        name = request.form['name']
        password = request.form['password']
        user = views.check_user(name)
        if len(user) > 0:
            if check_password_hash(user['passcode'], password):
                session['name'] = user['username']
                flash("Logged in successfully!", category='success')
                return redirect(url_for('home_page'))
            else:
                flash("Incorrect password!", category="error")    
                
    return render_template("login.html", boolean=True)

#### LOGOUT PAGE ####
@app.route('/logout')
def logout():
    return redirect(url_for('login'))

#### SIGN ####
@app.route('/sign', methods=["POST", "GET"])
def sign():
    if request.method == "POST":
        name = request.form.get('name')
        password = request.form.get('password')
        user = views.check_user(name)
        if user:
            flash("User already exists! Log in instead", category="error")
        elif len(name) < 7:
            flash("Username should be longer than 7 characters!", category = "error")
        elif len(password) < 7:
            flash("Password should be longer than 7 characters!", category="error")
        else:
            new_user = auth.User(username=name, password=generate_password_hash(password, method='sha256'))
            views.insert_user(new_user.username, new_user.password)
            user = views.check_user(new_user.username)
            session['name'] = user['username']
            return redirect(url_for('home_page'))

    return render_template("sign.html")


@app.route("/")
def home_page():
    if session.get("name", None) is not None:
        user = session.get("name")
        return render_template("index.html", user=user)
    return redirect(url_for('login'))

####SHOWING FILM PAGE####
@app.route('/film')
def film():
    film_data = views.fetch_film()
    lang_data = views.fetch_language()
    cat_data = views.fetch_category()
    return render_template("film.html", film_data=film_data, lang_data=lang_data, cat_data=cat_data)

#### ADDING FILM TABLE ####
@app.route('/get_film', methods=['POST'])
def get_film():
    if request.method == "POST":
        title = request.form['title']
        description = request.form['description']
        year = request.form['year_datepicker']
        category = request.form.get('cat_menu')
        if category == 'other':
            category = request.form['other']
            views.insert_category(category)
        language = request.form.get('lang_menu')
        if language == 'Other':
            language = request.form['Other']
            views.insert_language(language)
        rating = request.form.get('ratings')
        rent_dur = request.form['rent_dur']
        rent_rate = request.form['rent_rate']
        length = request.form['length']
        cost = request.form['cost']
        cat_id = views.fetch_category().index(category) + 1
        lang_id = views.fetch_language().index(language) + 1
        views.insert_film(title, description, year, lang_id, rent_dur,
                    rent_rate, length, cost, rating)
        views.fetch_id(title,cat_id)

        return redirect(url_for('film'))

    return render_template("film.html")

#### UPDATING FILM TABLE ####
@app.route('/update', methods=['POST', 'GET'])
def update():
    if request.method == "POST":
        film_id = request.form['id']
        title = request.form['title']
        description = request.form['description']
        year = request.form['year_datepicker']
        category = request.form.get('cat_menu')
        if category == 'other':
            category = request.form['other']
            views.insert_category(category)
        language = request.form.get('lang_menu')
        if language == 'Other':
            language = request.form['Other']
            views.insert_language(language)
        rating = request.form.get('ratings')
        rent_dur = request.form['rent_dur']
        rent_rate = request.form['rent_rate']
        length = request.form['length']
        cost = request.form['cost']
        feature = ",".join(request.form.getlist('features'))
        cat_id = views.fetch_category().index(category) + 1
        lang_id = views.fetch_language().index(language) + 1

        views.update_film(title, description, year, lang_id,
                    rent_dur, rent_rate, length, cost, rating, film_id)
        
        views.update_film_category(cat_id, film_id)
        
        return redirect(url_for('film'))

    return render_template("film.html")

#### DELETING FROM TABLE ####
@app.route('/delete/<string:film_id>', methods=['GET'])
def delete(film_id):
    views.delete_film(film_id)
        
    return redirect(url_for('film'))
################## STAFF TABLE ##########
#### SHOWING STAFF PAGE #####
@app.route('/staff/')
def staff():
    datas = views.fetch_staff()
    countries = views.fetch_country()
    
    return render_template('staff.html', tuples=datas, country=countries)
##### ADDING NEW STAFF ####
@app.route('/add_new_staff', methods=['GET', 'POST'])
def add_new_staff():
    first_name = request.form['fname']
    last_name = request.form['lname']
    email = request.form['email']
    username = request.form['username']
    password = request.form['password']
    storeId = request.form['st_Address']
    cityId = request.form['city']
    addressId = request.form['h_Address']
    if addressId == "other":
        address = request.form['adding_other_Home_Address']
        addressId = views.insert_address(cityId,address)
        addressId = addressId['address_id']
    views.insert_staff(first_name, last_name, addressId, email, storeId, username, password)
    return redirect('/staff/')
#### DELETING FROM STAFF ####
@app.route('/delete_from_staff/<int:id>')
def delete_from_staff(id):
    views.delete_staff(id)
    return redirect('/staff/')

#### UPDATING FROM STAFF ####
@app.route('/update_staff/', methods=['GET', 'POST'])
def update_staff():
    id = request.form['id']
    int_id = int(id)
    fname = request.form['f_name']
    lname = request.form['l_name']
    email = request.form['em']
    username = request.form['usn']
    stAdd = request.form['storeAddress']
    hAdd = request.form['homeAddress']
    views.update_staff(fname, lname, email, stAdd, username, hAdd, int_id)
    
    return redirect('/staff/')

#### FETCHING CITY FOR DYNAMIC SHOW ####
@app.route('/staff_city/<id>')
def staff_city(id):
    city = views.fetch_city(id)
    cityArray = []
    for row in city:
        cityObj = {
            'city_id': row['city_id'],
            'city': row['city']}
        cityArray.append(cityObj)
    return jsonify({'staff_citycountry': cityArray})
#### FETCHING STORE FOR DYNAMIC SHOW ####
@app.route('/staff_store_address/<id>')
def staff_store_address(id):
    store_Address = views.fetch_address(id)
    store_Address_Arr = []
    for row in store_Address:
        cityObj = {
            'store_id': row['store_id'],
            'address': row['address']}
        store_Address_Arr.append(cityObj)
    return jsonify({'staff_store_address': store_Address_Arr})

#### FETCHING  ADDRESS FOR DYNAMIC SHOW ####
@app.route('/staff_address/<id>')
def staff_address(id):
    store_Address = views.fetch_staff_address(id)
    store_Address_Arr=[]
    for row in store_Address:
        cityObj={
            'address_id':row['address_id'],
            'address':row['address']}
        store_Address_Arr.append(cityObj)
    return jsonify({'staff_address':store_Address_Arr})

#### FETCHING STAFF HOME ADDRESS FOR DYNAMIC SHOW ####
@app.route('/staff_home_address/<id>')
def staff_home_address(id):
    home_Address = views.fetch_Home_Address(id)
    home_Address_Arr=[]
    for row in home_Address:
        cityObj={
            'address_id':row['address_id'],
            'address':row['address']}
        home_Address_Arr.append(cityObj)
    return jsonify({'staff_home_address':home_Address_Arr})

# @app.route('/show_store/')
# def show_store():
#     store = views.fetch_store()
#     countries = views.showCountry()
#     return render_template('store.html',tuples = store,country=countries)
#### FETCHING CITY FOR DYNAMIC SHOW ####
@app.route('/city/<get_city>')
def city(get_city):
    city = views.getCity(get_city)
    cityArray=[]
    for row in city:
        cityObj={
            'city_id':row['city_id'],
            'city':row['city']}
        cityArray.append(cityObj)
    return jsonify({'citycountry':cityArray})





