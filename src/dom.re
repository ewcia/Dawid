type classList;

type node = {.
  [@bs.set] "style": string,
  "classList": option(array(classList))
};
[@bs.val] external document: node = "document";

type window = {.
  [@bs.meth] "requestAnimationFrame": ((unit => unit) => unit)
};
[@bs.val] external window: window = "window";

type eventCb('a) = 'a => unit;

type eventName;
let animationEnd: eventName = [%bs.raw {| "animationend" |}];
let mouseEnter: eventName = [%bs.raw {| "mouseenter" |}];
let mouseLeave: eventName = [%bs.raw {| "mouseleave" |}];
let click: eventName = [%bs.raw {| "click" |}];

type event = {.
  "target": node,
  "path": array(node)
};

[@bs.send] external removeChild: (node, node) => unit = "removeChild";
[@bs.send] external appendChild: (node, node) => unit = "appendChild";
[@bs.send] external cloneNode: (node, bool) => node = "cloneNode";
[@bs.send] external setAttribute: (node, string, string) => unit = "setAttribute";
[@bs.send] external getAttribute: (node, string) => option(string) = "getAttribute";
[@bs.send] external insertBefore: (node, node, node) => unit = "insertBefore";
[@bs.send] external addEventListener: (node, eventName, (event => unit), bool) => unit = "addEventListener";
[@bs.send] external removeEventListener: (node, eventName, (event => unit)) => unit = "removeEventListener";

[@bs.send] external querySelector: (node, string) => node = "querySelector";
[@bs.send] external querySelectorAll: (node, string) => array(node) = "querySelectorAll";

[@bs.val] external children: (node) => array(node) = "children";

[@bs.scope "classList"][@bs.send] external classListRemove: (node, string) => unit = "remove";
[@bs.scope "classList"][@bs.send] external classListAdd: (node, string) => unit = "add";
[@bs.scope "classList"][@bs.send] external classListContains: (node, string) => bool = "contains";

type date = {.
  [@bs.meth] "getTime": (unit => int)
};

[@bs.new] external createDate: unit => date = "Date";
