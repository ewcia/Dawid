type slideState = Slide | SlideDown | Inactive;

type slide = {
  isShown: bool,
  node: Dom.node,
  slideState: slideState,

  heading: string,
  subheading: string
};

type state = {
  slides: list(slide),
  container: Dom.node,
  observer: Observable.observer(state, string),
  observable: Observable.observable(state, string)
};

let animationEnd = Dom.animationEnd;


let clear = (state: state): state => {
  let slides = List.map(s => {
    Dom.removeChild(state.container, s.node);
    {...s, isShown: false}
  }, state.slides);

  {...state, slides};
};

let clearState = (slides: list(slide)): list(slide) => {
  List.map(slide => {
    switch (slide.slideState) {
      | Slide => Dom.classListRemove(slide.node, "slide")
      | SlideDown => Dom.classListRemove(slide.node, "slide-down")
      | Inactive => ()
    };

    {...slide, slideState: Inactive};
  }, slides);
};

let showImage = (state: state, idx: int): state => {
  let slides = List.mapi((i, slide) => {
    if (i === idx) {
      Dom.appendChild(state.container, slide.node);
      {...slide, isShown: true};
    } else {
      slide;
    }
  }, state.slides);

  let newState = {...state, slides};
  state.observer#next(newState);
  newState;
};

let replaceNth = (el: 'a, idx: int, list: list('a)): list('a) => {
  List.mapi((i, ith) => i === idx ? el : ith, list);
};

let transitionBackwards = (state: state, idx: int): state => {
  let slides = List.mapi((i, slide) => {
    if (slide.isShown === true) {
      let rec animationEndCb = () => {
        Dom.removeChild(state.container, slide.node);
        let s = replaceNth({...slide, isShown: false, slideState: Inactive});
        let s' = replaceNth({...slide, isShown: true, slideState: Inactive});

        state.observer#next({...state, slides: s'(idx, s(i, state.slides))});
        Dom.removeEventListener(slide.node, animationEnd, animationEndCb);
      };
      Dom.addEventListener(slide.node, animationEnd, animationEndCb, false);

      Dom.classListAdd(slide.node, "slide-down");
      {...slide, slideState: SlideDown};
    } else if (i === idx) {
      let firstSlide = Array.get(Dom.children(state.container), 0);
      Dom.insertBefore(slide.node, firstSlide);
      Dom.classListAdd(slide.node, "slide-down");

      {...slide, isShown: true, slideState: SlideDown};
    } else {
      slide;
    }
  }, state.slides);

  {...state, slides};
};

let create = (selector: string): state => {
  let container = Dom.querySelector(Dom.document, selector);
  let slideNodes: array(Dom.node) = [%bs.raw {| container.children |}];

  let (observer, observable) = Observable.create();

  let slides = List.map(slide => {
    {
      isShown: false,
      node: slide,
      slideState: Inactive,
      heading: "",
      subheading: ""
    }
  }, Array.to_list(slideNodes));

  clear({container, slides, observer, observable});
};
