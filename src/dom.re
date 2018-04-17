type node;
[@bs.val] external document: node = "document";

type eventCb('a) = 'a => unit;

type eventName;
[@bs.val] external animationEnd: eventName = "animationend";
[@bs.val] external click: eventName = "click";

[@bs.send] external removeChild: (node, node) => unit = "";
[@bs.send] external appendChild: (node, node) => unit = "";
[@bs.send] external insertBefore: (node, node, node) => unit = "";
[@bs.send] external addEventListener: (node, eventName, (unit => unit), bool) => unit = "";
[@bs.send] external removeEventListener: (node, eventName, (unit => unit)) => unit = "";

[@bs.send] external querySelector: (node, string) => node = "";
[@bs.val] external children: (node) => array(node) = "children";

[@bs.scope "classList"][@bs.send] external classListRemove: (node, string) => unit = "remove";
[@bs.scope "classList"][@bs.send] external classListAdd: (node, string) => unit = "add";
