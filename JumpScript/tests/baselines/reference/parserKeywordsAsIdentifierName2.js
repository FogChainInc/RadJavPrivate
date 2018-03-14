//// [parserKeywordsAsIdentifierName2.jump]
// 'public' should be marked unusable, should complain on trailing /*
a.public /*

//// [parserKeywordsAsIdentifierName2.js]
// 'public' should be marked unusable, should complain on trailing /*
a.public; /*
