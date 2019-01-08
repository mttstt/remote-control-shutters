// Copyright 2016, mtt stt, Inc.
//
// Actions on Google Node.js client library
//

'use strict';

const functions = require('firebase-functions');

const {
  dialogflow,
} = require('actions-on-google');

const app = dialogflow({
  debug: true,
});

const temperatura = () => {
  // ...
};

const umidita = () => {
  // ...
};


app.intent('Ciao Mora', conv => {
  conv.close('Ciao mora presente!')
})

app.intent('temperature', async (conv,{stanza}) => {
  const temp = await temperatura();
  const humi = await umidita();
  conv.close(`La tempertura in ${name} è pari a ${temp} e l'umidità è pari al ${humi} percento!`);
});

exports.dialogflowFirebaseFulfillment = functions.https.onRequest(app);
