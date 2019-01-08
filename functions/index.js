// Copyright 2016, mtt stt, Inc.
//
// Actions on Google Node.js client library
//

'use strict';

const express = require('express');
const bodyParser = require('body-parser');
const {dialogflow} = require('actions-on-google');

const app = dialogflow({debug: true});

const map = { "sala": "192.168.1.1", "cucina": "192.168.1.113", "camera matrimoniale": "192.168.1.113"};

const temperatura = () => {
  var ip = '192.168.1.112';
  try { var response = HTTP.get("http://"+ip+"/json", {});     
      if (response.data !== null) {
                var data = [response.data.Sensors[0].Temperature, response.data.Sensors[0].Humidity];
                return data;
             } else {console.log('Warning response.data null:',ip); }
	}
  catch(e){ console.log('Warning reading sensor:',ip);}
};

app.intent('Ciao Mora', conv => {
  conv.close('Mora viva e vegeta!')
})

app.intent('temperature', async (conv,{stanza}) => {
  conv.ask(`Connessione mora ok.`)
  const data = await temperatura();
  conv.close(`La tempertura in ${name} è pari a ${data[0]} e l'umidità è pari al ${data[1]} percento.`);
});


const expressApp = express().use(bodyParser.json());
expressApp.post('/fulfillment', app);
expressApp.listen(7000);     
