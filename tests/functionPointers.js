
var cBind = require('../build/Debug/cbind_core.node');

var functionPointers = cBind.functionPointers;

var mocha = require('mocha');

var chai = require('chai');
var assert = chai.assert;
chai.should();

describe('function pointers', function() {

  it('should call complex definition', function() {

    var innerCalled = false;
    var customPrintCalled = false;

    functionPointers.custom_print = function(a, b, c) {
      a.should.equal('FINAL');
      b.should.equal('TEST');
      c.should.equal(44);
      customPrintCalled = true;
    };

    functionPointers.catch_and_return(function(a, b, c) {

      a.should.equal('hello');
      b.should.equal('my');
      cBind.derefInt(c).should.equal(42);
      innerCalled = true;

    }, "str1", "str2", cBind.createInt(30))("FINAL", "TEST", 44);

    innerCalled.should.equal(true);
    customPrintCalled.should.equal(true);

  });

  it('should get the same object after setting to the variable', function() {

    var f = function(a, b, c) {};

    functionPointers.custom_print = f;

    functionPointers.custom_print.should.equal(f);



  });
});