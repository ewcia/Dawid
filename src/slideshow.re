type node;

type slideState = Slide | SlideDown | Inactive;

type eventCb('a) = 'a => unit;

type slide = {
  isShown: bool,
  node: node,
  slideState: slideState,

  heading: string,
  subheading: string
};

type state = {
  slides: list(slide),
  container: node,
  callbacks: list(eventCb(int))
};

type eventName;
[@bs.val] external animationEnd: eventName = "animationend";

[@bs.send] external removeChild: (node, node) => unit = "";
[@bs.send] external appendChild: (node, node) => unit = "";
[@bs.send] external insertBefore: (node, node) => unit = "";
[@bs.send] external addEventListener: (node, eventName, (unit => unit), bool) => unit = "";
[@bs.send] external removeEventListener: (node, eventName, (unit => unit)) => unit = "";
[@bs.scope "document"][@bs.val] external querySelector: (string) => node = "";

[@bs.scope "classList"][@bs.send] external classListRemove: (node, string) => unit = "remove";
[@bs.scope "classList"][@bs.send] external classListAdd: (node, string) => unit = "add";

let clear = (state: state): state => {
  let slides = List.map(s => {
    removeChild(state.container, s.node);
    {...s, isShown: false}
  }, state.slides);

  {...state, slides};
};

let clearState = (slides: list(slide)): list(slide) => {
  List.map(slide => {
    switch (slide.slideState) {
      | Slide => classListRemove(slide.node, "slide")
      | SlideDown => classListRemove(slide.node, "slide-down")
      | Inactive => ()
    };

    {...slide, slideState: Inactive};
  }, slides);
};

let showImage = (state: state, idx: int): state => {
  let slides = List.mapi((i, slide) => {
    if (i === idx) {
      appendChild(state.container, slide.node);
      {...slide, isShown: true};
    } else {
      slide;
    }
  }, state.slides);

  {...state, slides};
};

let transitionBackwards = (state: state, idx: int): state => {
  let slides = List.mapi((i, slide) => {
    if (slide.isShown === true) {
      classListAdd(slide.node, "slide-down");
      {...slide, slideState: SlideDown};
    } else if (i === idx) {
      insertBefore(state.container, slide.node);
      classListAdd(slide.node, "slide-down");

      let rec animationEndCb = () => {
        removeChild(state.container, slide.node);
        removeEventListener(slide.node, animationEnd, animationEndCb);
      };
      addEventListener(slide.node, animationEnd, animationEndCb, false);

      {...slide, isShown: true, slideState: SlideDown};
    } else {
      slide;
    }
  }, state.slides);

  {...state, slides};
};

let create = (selector: string): state => {
  let container = querySelector(selector);
  let slideNodes: array(node) = [%bs.raw {| container.children |}];

  let slides = List.map(slide => {
    {
      isShown: false,
      node: slide,
      slideState: Inactive,
      heading: "",
      subheading: ""
    }
  }, Array.to_list(slideNodes));

  clear({container, slides, callbacks: []});
};
