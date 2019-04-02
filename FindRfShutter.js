#!/usr/bin/env node
'use strict';
const fs = require('fs');
//var exec = require('child_process').exec;
var command;

function dec2bin(dec){
     return (dec >>> 0).toString(2);
}

function go () {
        var i;
        var preambolo ="110011000000";
        var direction="1001"; //up:1001 do:0010 st:0110
        var zero = "0000000000000000";
        var fine = "101000100000000000";
        var noti = ['0001100101010001','1001100100010001','0101100101100001','1101100100100001','0011100101000001','1011100100000001','0111100101111110','11$
        for (i = 1; i < 65536; i++)
        {
         var id = dec2bin(-i).slice(-16);
         if  (!(noti.indexOf(id) > -1))
          { var tentativo = preambolo+direction+zero+id+fine;
            var command = '~/remote-control-shutters/send1.py '+ tentativo;
           console.log(command);
                    
           require('child_process').execSync(command, {stdio: 'inherit'} );        
                    
           //exec(command, function callback(error, stdout, stderr){
           //          console.log('stdout: ' + stdout + i + id);
           //          console.log('stderr: ' + stderr);
           //          console.log('exec error: ' + error);
           //});
                    
         }
      }
}
go();
