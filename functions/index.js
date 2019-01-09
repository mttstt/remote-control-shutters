// Copyright 2016, mtt stt, Inc.
//
// Actions on Google Node.js client library
//

'use strict';

const express = require('express');
const bodyParser = require('body-parser');
const {dialogflow} = require('actions-on-google');
const app = dialogflow({debug: true});
var http = require('http');
const map = { "sala": "192.168.1.112", "cucina": "192.168.1.113", "camera matrimoniale": "192.168.1.113"};

function temperatura(stanza) {
   return new Promise(((resolve) => { 
	var ip = '192.168.1.112';
	http.get("http://"+ip+"/json", (res) => {
	  const { statusCode } = res;
	  const contentType = res.headers['content-type'];
	  let error;
	  if (statusCode !== 200) {
	    error = new Error('Request Failed.\n' +
			      `Status Code: ${statusCode}`);
	  } else if (!/^application\/json/.test(contentType)) {
	    error = new Error('Invalid content-type.\n' + `Expected application/json but received ${contentType}`);
	  }
	  if (error) {
	    console.error(error.message);
	    res.resume();
	    return;
	  }
	  res.setEncoding('utf8');
	  let rawData = '';
	  res.on('data', (chunk) => { rawData += chunk; });
	  res.on('end', () => {
	    try {
	      const parsedData = JSON.parse(rawData);
	      var data = [parsedData.Sensors[0].Temperature, parsedData.Sensors[0].Humidity];
	      console.log('====> ',data);
	      resolve(data);
	    } catch (e) {
	      console.error(e.message);
	    }
	  });
	  }).on('error', (e) => { console.error(`Got error: ${e.message}`);});
  }));
}


app.intent('Ciao Mora', conv => {
  conv.close(`Mora viva e vegeta!`)
})


app.intent('dimmi la temperatura in camera', (conv,{stanza}) => {
  return temperatura(stanza).then((data) => {
    conv.close(`La tempertura in ${stanza} è pari a ${data[0]} gradi centigradi e l'umidità è pari al ${data[1]} percento.`);
  })
})	

const expressApp = express().use(bodyParser.json());
expressApp.post('/fulfillment', app);
expressApp.listen(7000);

