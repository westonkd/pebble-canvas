class ApiHelper {
  constructor(baseUrl, token) {
    this.baseUrl = baseUrl; 
    this.token = token;
    console.log("IN CONSTRUCTOR!");
  }
  
  fetchRespone(endpoint, type) {
    var url = this.baseUrl + endpoint;
    this.xhrRequest(url, type, this.handleResponse);
  }
  
  handleResponse(responseText) {
    console.log(responseText);
  }
  
  xhrRequest(url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
      callback(this.responseText);
    };
    xhr.open(type, url);
    
    xhr.setRequestHeader("Authorization","Bearer " + this.token);
    
    xhr.send();
  }
}

// Listen for when the app is opened
Pebble.addEventListener('ready', function(e) {
  var baseUrl = "http://wdransfield.instructure.com";
  var token = "9336~4Q7vauyL6JxKma2JsE9dcWpoZb0p79f4rBHx9eD9KYvgpoKxLxgCWNTGX6W0CadA";
  var apiFilter = new ApiHelper(baseUrl, token);
  
  apiFilter.fetchRespone("/api/v1/users/self/upcoming_events", "GET");
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e){
  console.log("Received AppMessage");
});