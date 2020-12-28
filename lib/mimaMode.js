// CodeMirror, copyright (c) by Marijn Haverbeke and others
// Distributed under an MIT license: https://codemirror.net/LICENSE

(function(mod) {
  if (typeof exports == "object" && typeof module == "object") // CommonJS
  mod(require("../../lib/codemirror"));
  else if (typeof define == "function" && define.amd) // AMD
  define(["../../lib/codemirror"], mod);
  else // Plain browser env
  mod(CodeMirror);
})(function(CodeMirror) {
"use strict";

CodeMirror.defineMode('mima', function(_config, parserConfig) {
  var keywords1 = /^(ldc|ldv|stv|add|and|or|xor|eql|halt|not|rar|ldiv|stiv|printakku|print)\b/i;
  var keywords2 = /^(call|jmp|jmn)\b/i;

  var variables1 = /^(af?|bc?|c|de?|e|hl?|l|i[xy]?|r|sp)\b/i;
  var variables2 = /^(n?[zc]|p[oe]?|m)\b/i;
  var errors = /^([hl][xy]|i[xy][hl]|slia|sll)\b/i;
  var numbers = /^([\da-f]+h|[0-7]+o|[01]+b|\d+d?)\b/i;

  return {
    startState: function() {
      return {
        context: 0
      };
    },
    token: function(stream, state) {
      if (!stream.column())
        state.context = 0;

      if (stream.eatSpace())
        return null;

      var w;

      if (stream.eatWhile(/\w/)) {
        if (stream.eat('.')) {
          stream.eatWhile(/\w/);
        }
        w = stream.current();

        if ((state.context == 1 || state.context == 4) && variables1.test(w)) {
        state.context = 4;
        return 'var2';
        }

        if (state.context == 2 && variables2.test(w)) {
        state.context = 4;
        return 'var3';
        }

        if (keywords1.test(w)) {
        state.context = 1;
        return 'keyword';
        } else if (keywords2.test(w)) {
        state.context = 2;
        return 'keyword';
        } else if (state.context == 4 && numbers.test(w)) {
        return 'number';
        }

        if (errors.test(w))
        return 'error';

        if (stream.match(numbers))
          return 'number';

        return null;

      } else if (stream.eat('/') && stream.eat('/')) {
        stream.skipToEnd();
        return 'comment';
      } else if (stream.eat('"')) {
        while (w = stream.next()) {
          if (w == '"')
            break;

          if (w == '\\')
            stream.next();
        }
        return 'string';
      } else if (stream.eat('\'')) {
        if (stream.match(/\\?.'/))
          return 'number';
      } else if (stream.eat('.') || stream.sol() && stream.eat('#')) {
        state.context = 5;

        if (stream.eatWhile(/\w/))
          return 'def';
      } else if (stream.eat('$')) {
        if (stream.eatWhile(/[\da-f]/i))
          return 'number';
      } else if (stream.eat('%')) {
        if (stream.eatWhile(/[01]/))
          return 'number';
      } else {
        stream.next();
      }
      return null;
    }
  };
});

CodeMirror.defineMIME("text/mima", "mima");
CodeMirror.defineMIME("text/mima", { name: "mima", mima: true });

});