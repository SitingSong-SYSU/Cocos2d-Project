var router = require('koa-router')();
var Puid = require('puid');

var puid = new Puid();

var users = [];

router.post('/', async (ctx, next) => {
  var session = puid.generate();
  for (var i = 0; i < users.length; i++) {
    if (users[i].username == ctx.request.body.username) {
      console.log(users);
      ctx.body = users[i].session;
      return;
    }
  }
  users.push({
    username: ctx.request.body.username,
    session
  });
  console.log(users);
  ctx.body = session;
});

router.post('/score', (ctx, next) => {
  var username = ctx.request.body.username;
  var session = ctx.request.body.cookie;
  for (var i = 0; i < users.length; i++) {
    if (users[i].username == username && users[i].session == session) {
      if (!users[i].score || users[i].score < ctx.request.body.score) {
        users[i].score = ctx.request.body.score;
        console.log(users);
        ctx.body = "success";
        return;
      }
    }
  }
  console.log(users);
  ctx.body = "fail";
});

function comp(a, b) {
  return b.score - a.score;
}
router.get('/score', (ctx, next) => {
  var ret = [];
  for (var i = 0; i < users.length; i++) {
    if (!!users[i].score) {
      ret.push(users[i]);
    }
  }
  ret.sort(comp);
  ctx.body = '';
  for (var i = 0; i < 10 && i < ret.length; i++) {
    ctx.body += ret[i].username + ':' + ret[i].score + '|';
  }
});

module.exports = router;
