//Dynamic Country - City - Address selection
let country_select = document.querySelectorAll('.country');
let city_select = document.querySelectorAll('.city');
let st_address_select = document.querySelectorAll('.st_Address');
let home_address_select = document.querySelectorAll('.h_Address');
let updateBtn = document.querySelectorAll('.updateRow');
let city_option_html = '';
const staffTable = document.querySelector('#staff_table')
let staffArr=[];

for(let i=0;i<country_select.length;i++){
  country_select[i].onchange = function(){
    country = country_select[i].value;
    fetch('/staff_city/'+Number(country)).then(function(response){
      response.json().then(function(data){
        city_option_html = '';
        for(let city of data.staff_citycountry){
          city_option_html+=`<option value=${city.city_id}> ${city.city}</option>`;
        }
        city_select[i].innerHTML = city_option_html;
  
        ////storeAddress
        let store_address_option_html = '';
        city = city_select[i].value;
        fetch('/staff_store_address/'+Number(city)).then(function(response){
          response.json().then(function(data){
            store_address_option_html = '';
            for(let address of data.staff_store_address){
              store_address_option_html+=`<option value=${address.store_id}> ${address.address}</option>`;
            }
            st_address_select[i].innerHTML = store_address_option_html;
          });
        let home_address_option_html = '';
        city = city_select[i].value;
        fetch('/staff_home_address/'+Number(city)).then(function(response){
          response.json().then(function(data){
            
            home_address_option_html = '';
            for(let address of data.staff_home_address){
              home_address_option_html+=`<option value=${address.address_id}> ${address.address}</option>`;
            }
            home_address_select[i].innerHTML = home_address_option_html;
            if(i==0)
              home_address_select[i].innerHTML += `<option value="other">Other</option>`;
          });
        });   
        });
      });
    });
  }
}


  for(let hmAddressOption of home_address_select){
    hmAddressOption.onchange = function(){
      if(hmAddressOption.value == "other"){
        document.querySelector('#add_New_Home_Adrres').style.visibility = "visible";
        
      }
      else{
        document.querySelector('#add_New_Home_Adrres').style.visibility = "hidden";
      }
    }
  }



       
//Form - Open Close
function openForm(){
    document.querySelector('#modal').style.display = "block";
  }
  window.onclick = function(event){
    if(event.target == document.querySelector('.modal')){
      document.querySelector('#modal').style.display = "none";
    }
    else if(event.target == document.querySelector('#updateModal')){
      document.querySelector("#updateModal").style.display="none";
    }
}
//////
function openUpdateForm(){
  document.querySelector("#updateModal").style.display="block";
}


//Update Modal Open Close



let row_Lenght = staffTable.rows.length;


for(let i=1;i<row_Lenght;i++){
  staffArr.push(
    {
      id:Number(staffTable.rows[i].cells.item(0).innerHTML),
      firstName:staffTable.rows[i].cells.item(1).innerHTML,
      lastName:staffTable.rows[i].cells.item(2).innerHTML,
      email:staffTable.rows[i].cells.item(3).innerHTML,
      username:staffTable.rows[i].cells.item(4).innerHTML,
      country:staffTable.rows[i].cells.item(5).innerHTML,
      city:staffTable.rows[i].cells.item(6).innerHTML,
      store:staffTable.rows[i].cells.item(7).innerHTML,
      Home:staffTable.rows[i].cells.item(8).innerHTML,
    }
  );
}
for(let btn of updateBtn){
  btn.addEventListener('click',function(){
    document.querySelector("#updateModal").style.display="block";
    staff = showStaff(btn.id);
    document.querySelector('#id').value=staff.id;
    document.querySelector('#f_name').value=staff.firstName;
    document.querySelector('#l_name').value=staff.lastName;
    document.querySelector('#em').value=staff.email;
    document.querySelector('#usn').value = staff.username;
    
  });
}

function showStaff(id){
  for(let staff of staffArr){
    if(staff.id == id){
      return staff;
    }
  }
}

function findID(countryName){
  for(let opt of document.querySelector('#u_country').options){
    if(countryName == opt.innerHTML){
      return Number(opt.value);
    }
  }
}

