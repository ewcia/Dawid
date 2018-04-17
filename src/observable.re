type subscription;

type observable('a, 'b) = {.
  subscribe: (('a => unit), ('b => unit), (unit => unit)) => subscription
};
type observer('a, 'b) = {.
  closed: ref(bool),
  next: 'a => unit,
  error: 'b => unit,
  complete: unit => unit
};

[@bs.scope "Observable"][@bs.module "@reactivex/rxjs/dist/cjs/Observable"][@bs.val] external createObservable: (observer('a, 'b) => unit) => observable('a, 'b) = "create";

type create('a, 'b) = (unit) => (observer('a, 'b), observable('a, 'b));

let create: create('a, 'b) = () => {
  let obs: ref(option(observer('a, 'b))) = ref(None);
  let createResult = createObservable((o) => {
    Js.log("creator");
    obs := Some(o);
  });

  Js.log(obs);
  (Js_option.getExn(obs^), createResult);
};
