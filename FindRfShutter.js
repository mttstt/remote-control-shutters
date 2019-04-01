#!/usr/bin/env node

// Copyright 2016, mtt stt, Inc.
//
// Accept HTTPS and HTTP calls.
// HTTPS POST calls 
//   are used in conjunction with "Actions on Google" Node.js client library for temperature and humidity in rooms
// 
// HTTP GET calls 
//   are used in conjuction with IFTTT for move shutters and gate
//   Here some examples oh http GET calls:
//     http://XXX.dlinkddns.com:7000/?shutter=do6&up7&do8
//     http://XXX.dlinkddns.com:7000/?shutter=canc
//     http://XXX.dlinkddns.com:7000/?shutter=up0

'use strict';
const fs = require('fs');
var exec = require('child_process').exec;
var command;

function dec2bin(dec){
     return (dec >>> 0).toString(2);
}
   
function go(){
        String preambolo="110011000000";
        String direction="1001"; //up:1001 do:0010 st:0110
        String zero = "0000000000000000";
        String fine = "101000100000000000";
        String noti = ['0001100101010001','1001100100010001','0101100101100001','1101100100100001','0011100101000001','1011100100000001','0111100101111110','1111100100111110','0000010101011110'];
        for (i = 0; i < 65536; i++)
        {
         String id = dec2bin(-i).slice(-16);
         if ( !(noti.indexOf(id) > -1) )
          { String tentativo = preambolo+direction+zero+id+fine;
            command = '~/remote-control-shutters/send.py '+ tentativo;
            exec(command, function callback(error, stdout, stderr)
              { console.log('stdout: ' + stdout+stderr);
                //console.log('stderr: ' + stderr);
                console.log("Tentativo "+ i + " - ", id ); 
              }        
          }     
        }
}   
go();
