type subscription;
type _observer('a, 'b) = {.
  "closed": ref(bool),
  [@bs.meth] "next": 'a => unit,
  [@bs.meth] "error": 'b => unit,
  [@bs.meth] "complete": unit => unit
};

type observer('a, 'b) = {
  closed: ref(bool),
  next: 'a => unit,
  error: 'b => unit,
  complete: unit => unit
};
type observable('a, 'b) = {.
  [@bs.meth] "subscribe": (('a => unit), ('b => unit), (unit => unit)) => subscription
};

[@bs.scope "Observable"][@bs.module "@reactivex/rxjs/dist/cjs/Observable"][@bs.val] external createObservable: (_observer('a, 'b) => unit) => observable('a, 'b) = "create";

type create('a, 'b) = (unit) => (observer('a, 'b), observable('a, 'b));

let create: create('a, 'b) = () => {
  let realObservers: ref(list(_observer('a, 'b))) = ref([]);
  let observer: observer('a, 'b) = {
    closed: ref(false),
    next: (el) => {
      List.map(o => o##next(el), realObservers^) |> ignore;
    },
    error: (err) => {
      List.map(o => o##error(err), realObservers^) |> ignore;
    },
    complete: () => {
      List.map(o => o##complete(), realObservers^) |> ignore;
    }
  };
  let createResult = createObservable((o) => {
    realObservers := Js_list.cons(o, realObservers^);
  });

  (observer, createResult);
};
