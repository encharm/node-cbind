var cBind = require('../build/Debug/cbind_core.node');

var enums = cBind.enums;

var mocha = require('mocha');

var chai = require('chai');
var assert = chai.assert;
chai.should();

describe('enum support', function() {
  it('should properly get all enums', function() {
    assert.deepEqual(enums, {
      FIRST: 0,
      SECOND: 5,
      THIRD: 6,
      __END: 20,
      nsenums: {
        AAA: 0,
        BBB: 1
      }
    });
  });
});