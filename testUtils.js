
function objectMap(obj, operation) {
  var newObj = {};
  Object.keys(obj).forEach(function(key) {

    newObj[key] = operation(obj[key]);
  });

  return newObj;
}

function typeofObject(obj) {
  return objectMap(obj, function(elem) {
    return typeof elem;
  });
}


module.exports = {
  objectMap: objectMap,
  typeofObject: typeofObject
};