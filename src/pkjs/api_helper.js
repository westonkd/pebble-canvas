class ApiHelper {
  constructor(baseUrl, token) {
    this.baseUrl = baseUrl; 
    this.token = token;
    this.locale = 'en-us';
  }
  
  fetchRespone(endpoint, type) {
    var url = this.baseUrl + endpoint;
    this.xhrRequest(url, type, this.handleResponse);
  }
  
  handleResponse(responseText, self) {
    var eventNames = "";
    var eventTypes= "";
    var eventDates = "";
    var eventTimes = "";
    const data = JSON.parse(responseText);
    
    for (var i = 0; i < data.length; i++) {
      var eventDate;
      if (data[i].type === 'assignment') {
        eventDate = new Date(data[i].assignment.due_at);
      } else {
        eventDate = new Date(data[i].start_at);
      }
      
      eventNames += `${data[i].title}|`;
      eventTypes += `${data[i].type}|`;
      eventDates += `${eventDate.toLocaleString(self.locale, {month: "short"})} ${eventDate.getDate()}|`;
      eventTimes += `${eventDate.toLocaleString(self.locale, {hour: '2-digit', minute:'2-digit'})}|`
    }
    
    var dictionary = {
      'EVENT_NAMES': eventNames,
      'EVENT_TYPES': eventTypes,
      'EVENT_DATES': eventDates,
      'EVENT_TIMES': eventTimes,
      'EVENT_COUNT': data.length.toString()
    };
    
    Pebble.sendAppMessage(dictionary, self.successHandler, self.errorHandler);
  }

  errorHandler(e) {
    console.log("Error sending response to Pebble.");
    console.log(e);
  }

  successHandler(e) {
    console.log("Successfully sent response to Pebble.");
  }
  
  xhrRequest(url, type, callback) {
    var xhr = new XMLHttpRequest();
    var self = this;
    
    xhr.onload = function () {
      callback(this.responseText, self);
    };
    xhr.open(type, url);
    
    xhr.setRequestHeader("Authorization","Bearer " + this.token);
    
    xhr.send();
  }
}

function getUpcomingEvents() {
  var baseUrl = "http://wdransfield.instructure.com";
  var token = "9336~4Q7vauyL6JxKma2JsE9dcWpoZb0p79f4rBHx9eD9KYvgpoKxLxgCWNTGX6W0CadA";
  var apiFilter = new ApiHelper(baseUrl, token);
  
  apiFilter.fetchRespone("/api/v1/users/self/upcoming_events", "GET");
}

// Listen for when the app is opened
Pebble.addEventListener('ready', function(e) {
  getUpcomingEvents();
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e){
  console.log("Received AppMessage");
  getUpcomingEvents();
});