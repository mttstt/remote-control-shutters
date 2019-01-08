// Copyright 2016, mtt stt, Inc.
//
// Actions on Google Node.js client library
//

'use strict';

const express = require('express');
const bodyParser = require('body-parser');

const {dialogflow} = require('actions-on-google');
const app = dialogflow({debug: true});
const temperatura = () => {
  return '25';
};
const umidita = () => {
  return '80';
};
app.intent('Ciao Mora', conv => {
  conv.close('Ciao mora presente!')
})
app.intent('temperature', async (conv,{stanza}) => {
  const temp = await temperatura();
  const humi = await umidita();
  conv.close(`La tempertura in ${name} è pari a ${temp} e l'umidità è pari al ${humi} percento!`);
});


const expressApp = express().use(bodyParser.json());
expressApp.post('/fulfillment', app);
expressApp.listen(3000);
