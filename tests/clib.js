
var cBind = require('../build/Debug/cbind_core.node');

var c = cBind.clib;

var mocha = require('mocha');

var path = require('path');
var chai = require('chai');
var assert = chai.assert;
chai.should();

var fs = require('fs');

var typeofObject = require('../testUtils').typeofObject;

describe('native module', function() {
  it('should export proper API', function() {
    assert.deepEqual(typeofObject(c), {
      errno: 'number',
      fopen: 'function',
      fread: 'function',
      freadString: 'function',
      fclose: 'function'
    }, "expected cbind API");
  });
});

describe('fopen', function() {
  it('should fail to open non-existing file and set errno', function() {

    assert(c.errno === 0);

    var handle = c.fopen('someNonExistingAbsurdName', "r");;

    assert(c.errno !== 0);

    assert(handle === null)
  });


  var thisFile = path.join(__dirname, 'clib.js');
  it('should open and read this source with binary', function() {
    var handle = c.fopen(thisFile, "r");

    var buffer = new Buffer(64);;
    
    var contents = "";
    var n = 0;
    while( (n = c.fread(buffer, 1, handle)) > 0) {
      contents += buffer.toString('utf8', 0, n) ;
    }
    contents.should.equal(fs.readFileSync(thisFile, 'utf8'));
    assert(contents.length > 0);
    c.fclose(handle);
  });


  it('should open and read this source with text', function() {
  
    var handle = c.fopen(thisFile, "r");

    assert(handle !== null);
    var contents = "";
    var res;
    do {
      res = c.freadString(handle);
      contents += res.text;
    } while(res.retValue);

    contents.should.equal(fs.readFileSync(thisFile, 'utf8'));
    assert(contents.length > 0);
    c.fclose(handle);
  });


});