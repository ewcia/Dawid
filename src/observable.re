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

type observableInternalModule('a, 'b) = {.
  create: (observer('a, 'b) => unit) => observable('a, 'b)
};


[@bs.module "@reactivex/rxjs/es6/Observable"] external observableInternal: observableInternalModule('a, 'b) = "Observable";

type create('a, 'b) = (unit) => (observer('a, 'b), observable('a, 'b));

let create: create('a, 'b) = () => {
  let obs: ref(option(observer('a, 'b))) = ref(None);
  let createResult = observableInternal#create((o) => obs := Some(o));

  (Js_option.getExn(obs^), createResult);
};
