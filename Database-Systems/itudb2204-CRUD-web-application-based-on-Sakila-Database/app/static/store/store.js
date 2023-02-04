let country_select = document.querySelector('#country');
let city_select = document.querySelector('#city');
let st_select = document.querySelector('#st_Address')

country_select.onchange = function(){
    country = country_select.value;
    fetch('/city/' + Number(country)).then(function(response){
        response.json().then(function(data){
            let optionHTML='';
            for(let city of data.citycountry){
                optionHTML+=`<option value=${city.city_id}> ${city.city}</option>`;
            }
            city_select.innerHTML = optionHTML;
            let store_address_option_html = '';
            let city = city_select.value;
            fetch('/staff_address/'+Number(city)).then(function(response){
                response.json().then(function(data){
                    store_address_option_html = '';
                    for(let address of data.staff_address){
                        store_address_option_html+=`<option value=${address.address_id}> ${address.address}</option>`;
                    }
                    st_select.innerHTML = store_address_option_html;
                });
            });
        });
    });
}
city_select.onchange = function(){
    let store_address_option_html = '';
            let city = city_select.value;
            
            fetch('/staff_address/'+Number(city)).then(function(response){
                
                response.json().then(function(data){
                    store_address_option_html = '';
                    for(let address of data.staff_address){
                        store_address_option_html+=`<option value=${address.address_id}> ${address.address}</option>`;
                    }
                    st_select.innerHTML = store_address_option_html;
                });
            });
}

function showAddModal(){
    document.querySelector('#modal').style.display="block";
}
window.onclick = function(event){
    if(event.target == document.querySelector('.modal')){
      document.querySelector('#modal').style.display = "none";
    }
    else if(event.target == document.querySelector('#updateModal')){
      document.querySelector("#updateModal").style.display="none";
    }
}
