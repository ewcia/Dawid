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
  let realObservers: ref(list(observer('a, 'b))) = ref([]);
  let observer: observer('a, 'b) = {
    pub closed = ref(false);
    pub next = (el) => {
      List.map(o => o#next(el), realObservers^) |> ignore;
    };
    pub error = (err) => {
      List.map(o => o#error(err), realObservers^) |> ignore;
    };
    pub complete = () => {
      List.map(o => o#complete(), realObservers^) |> ignore;
    }
  };
  let createResult = createObservable((o) => {
    realObservers := Js_list.cons(o, realObservers^);
  });

  (observer, createResult);
};
