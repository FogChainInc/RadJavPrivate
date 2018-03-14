//// [parserArrowFunctionExpression5.jump]
(bar(x,
    () => {},
    () => {}
  )
)


//// [parserArrowFunctionExpression5.js]
(bar(x, function () { }, function () { }));
