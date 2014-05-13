var fs = require('fs');
var path = require('path');
var PEG = require('pegjs');
var fireTs = require('fire-ts');
var util = require('util');
var extend = require('extend');

var setParser = PEG.buildParser(fs.readFileSync(path.join(__dirname, 'set.peg'), 'utf8'));

//console.log(setParser);

function nameForFunction(func) {
  return func.name;
}

function produceFunction(func) {
  var name = nameForFunction(func);

  var out = "";
  out += "static inline NAN_METHOD(" + name + ") {\n";
  
  out += "}\n\n";
  return out;
}

function generateInit(baseName) {
  var out = "";
  out += "static inline void init_" + baseName +"(v8::Handle<v8::Object>& exports) {\n"

  out += "}\n";
  return out;
}

// function typeString(obj, options) {
//   var str = obj.type.name;
//   options = options || {};
//   if(obj.type.pointer && options !== '*')
//     str += obj.type.pointer;

//   if(options.param) {
//     str += ' ' + options.param;
//   }
//   return str;
//}

var typeString = require('nid-parser').typeString;

function typeIdentifier(type, options) {
  var str = ((typeof type === 'string') ? type : typeString(type, options));
  str = str.replace(/ /g, '_');
  str = str.replace(/,/g, '_');
  str = str.replace(/\(\*\)/g, '_funcptr_');
  str = str.replace(/\(/g, '_');
  str = str.replace(/\)/g, '_');

  str = str.replace(/::/g, '__');
  str = str.replace(/\*/g, ' ptr');
  return str.split(/\s+/).join('_');
}





console.log = console.warn;

var processedTypes = {};

var existingProcessedTypes = {
  'const_char_ptr': true,
  'char_ptr': true,
  'void_ptr': true
};

function processType(obj, options) {
  console.log("Pre process type", obj);
  if(! obj.type.pointer && !(obj.functionPointer || obj.type.functionPointer )) {
    console.log("Skipping type", obj);
    return;
  }
  var typeIdent = typeIdentifier(obj, options);

  if(existingProcessedTypes[typeIdent]) {
    console.log("Skipping due to already existing");
    return;
  }

  console.log("Type ident", typeIdent, processedTypes);
  if(!processedTypes[typeIdent]) {
    console.log("Adding type", typeIdent);

    obj.ifdefIdentifier = 'CBIND_HELPER_' + typeIdent.toUpperCase();
  }
  else {
    console.log("Type exists", typeIdent);
    return;
  }
  processedTypes[typeIdent] = obj;
  console.log("########ADDED", typeIdent);
}

function processFunction(func) {
  processType(func);
  if(func.parameters) {
    func.parameters.forEach(function(parameter) {
      processType(parameter);
    });
  }
}

function processVariable(_variable) {
  console.log(">>>>>>>>>>>>>>>>> ADDING VARIABLE", _variable);
  if(_variable.functionPointer) {
    processType(_variable, {forceParams: true});
  }
  else {
    processType(_variable);
  }
}

function processClass(_class) {
  processType({type: {
    name: _class.name,
    pointer: '*',
    nonPrimitive: true
  }});
}

function processNamespace(namespace) {
  namespace.declarations.forEach(function(declaration) {
    if(declaration.namespace)
      processNamespace(declaration.namespace);
    else if(declaration.function) {
      processFunction(declaration.function);
    }
    else if(declaration.struct || declaration['class']) {
      processClass(declaration.struct || declaration['class']);
    }
    else if(declaration.variable) {
      processVariable(declaration.variable);
    }

  });
}

function parseSet(setString, name2param)
{
  setParser.name2param = name2param;
  return setParser.parse(setString);
}

function processNidFile(file) {
  console.warn("Processing NID file:", file);
  var defs = fs.readFileSync(file);
  var base = path.basename(file);
  var baseName = path.basename(file, '.nid');
  var outHeader = path.join('build', 'cbind_' + baseName + '.h');

  var outGypi = path.join('build', 'cbind.gypi');

  var parser;
  try {
    parser = require('nid-parser');
  } catch(err) {
    console.log(err);
    process.exit(1);
  }

  function prepareForParse(str) {
    return str.replace(/\/\*[\s\S]*?\*\/|\/\/[^\n]*/gm, function(match) {
      return new Array(match.length + 1).join(' ');
    });
  }

  var nid;
  try {
    nid = parser.parse(prepareForParse(fs.readFileSync(file, 'utf8')));
  }
  catch(err) {
    console.log(err.line + ':' + err.offset, ':', err.message);
    process.exit(1);
  }


  var tplCache = {};
  var indent = 1;
  function processInclude(tplName, data) {

    console.log(">>>>>>>>>>>>>> PROCESS INCLUDE", tplName);

    ++indent;
    var indentStr = Array(indent).join('  ');

    var filename = path.join(__dirname, 'templates', tplName);
    if(!tplCache[tplName]) {

      tplCache[tplName] = fireTs.parseSync(filename, {
        indent: '  '
      });
//      console.log(tplCache[tplName]);
    }
    --indent;
    return tplCache[tplName](data, {
      render: function(f, f2, f3) {
        
        return processInclude(f, f2, f3).split(/\n/).map(function(line) {
          return indentStr + line;
        }).join('\n');
      }
    });
    
  }

  processNamespace(nid);
  processedTypes = Object.keys(processedTypes).map(function(key) {
    return processedTypes[key];
  });

  console.warn(util.inspect(nid, {depth: null}));


  var headerContent = "";
  try {
    headerContent = processInclude('header.fts', {
      baseName: baseName,
      declarations: nid.declarations,
      processedTypes: processedTypes,
      data: nid,
      upcase: function(str) {
        return str.toUpperCase();
      },
      typeString: typeString,
      typeIdentifier: typeIdentifier,
      parseSet: parseSet
    });

    console.warn(headerContent, {depth: null});
  } catch(err) {
    console.log(err.stack);
  }

  var buildDir = path.resolve('build');

  ['helpers.h', 'type_literal.h'].forEach(function(header) {
    fs.createReadStream(path.join(__dirname, header)).pipe(
      fs.createWriteStream(path.join(buildDir, header))
    );
  });
  
  //fs.writeFileSync(outHeader, '');
  fs.writeFile(outHeader, headerContent);
}

var globExpand = require('glob-expand');

module.exports = function() {
  var buildDir = path.resolve('build');
  var args = Array.prototype.slice.apply(arguments);
  //process.stdout.write("DUPA\n");

  var files = [];
  args.forEach(function(arg) {
    //process.stdout.write(globExpand(arg)+'\n');
    files = files.concat(globExpand(arg).toString('utf8').split(/,/));
  });
  //proess.stdout.write(files.join(',') + "\n");
  files.map(processNidFile);
  process.stdout.write(buildDir+'\n');
}
var defs = {};
try {
  defs = require('./build/Release/cbind_core.node')
} catch(err) {
}

module.exports = extend(module.exports, defs);