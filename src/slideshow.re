type slideState = Slide | SlideDown | Inactive;

type slide = {
  isShown: bool,
  node: Dom.node,
  slideState: slideState,

  heading: string,
  subheading: string
};


type state = {
  slides: array(slide),
  container: Dom.node,
  observer: Observable.observer(stateUpdate, string),
  observable: Observable.observable(stateUpdate, string)
}
and stateUpdate = State(state) | AnimationStart(int, state) | MoveTo(int) | Error(string);

let clear = (state: state): state => {
  let slides = Array.map(s => {
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
  Dom.appendChild(state.container, state.slides[idx].node);
  state.slides[idx] = {...state.slides[idx], isShown: true};

  state.observer.next(MoveTo(idx));
  state.observer.next(State(state));
  state;
};

let guardDuringTransition = (cb: ((state, int) => state)): ((state, int) => state) => {
  (state, idx) => {
    switch (Js_array.find(s => s.slideState !== Inactive, state.slides)) {
      | Some(_) => {
        state.observer.next(Error("Not initiating the transition during another transition"));
        state;
      }
      | None => {
        cb(state, idx);
      }
    };
  }
};

let _transition = (slideClass, slideState, state, idx) => {
  switch (Js_array.find(s => s.isShown, state.slides)) {
    | None => {
      state.observer.next(Error("Cannot transition because no slide is shown"));
      state;
    }
    | Some(shownSlide) => {
      let slideToShow = state.slides[idx];
      let shownSlideIdx = Js_array.findIndex((===)(shownSlide), state.slides);

      Dom.classListAdd(shownSlide.node, slideClass);
      Dom.classListAdd(slideToShow.node, slideClass);
      Dom.insertBefore(state.container, slideToShow.node, shownSlide.node);

      state.slides[shownSlideIdx] = {...shownSlide, slideState};
      state.slides[idx] = {...slideToShow, isShown: true, slideState};

      let rec animationEndCb = (_) => {
        Dom.removeChild(state.container, shownSlide.node);

        Dom.classListRemove(shownSlide.node, slideClass);
        Dom.classListRemove(slideToShow.node, slideClass);
        state.slides[shownSlideIdx] = {...shownSlide, isShown: false, slideState: Inactive};
        state.slides[idx] = {...state.slides[idx], slideState: Inactive};
        state.observer.next(State(state));

        Dom.removeEventListener(slideToShow.node, Dom.animationEnd, animationEndCb);
      };

      Dom.addEventListener(slideToShow.node, Dom.animationEnd, animationEndCb, false);
      state.observer.next(AnimationStart(idx, state));
      state;
    }
  };
};

let transitionBackwards = _transition("slide", Slide);
let transitionForwards = _transition("slide-down", SlideDown);

let transition = guardDuringTransition((state: state, idx: int): state => {
  switch (Js_array.findIndex(s => s.isShown, state.slides)) {
    | -1 => {
      state.observer.next(Error("Cannot transition because no slide is shown"));
      state;
    }
    | shownSlideIdx => {
      if (shownSlideIdx > idx) {
        transitionForwards(state, idx);
      } else if (shownSlideIdx < idx) {
        transitionBackwards(state, idx);
      } else {
        state.observer.next(Error("Same slide, not transitioning"));
        state;
      }
    }
  };
});

let create = (selector: string): state => {
  let container = Dom.querySelector(Dom.document, selector);
  let slideNodes: array(Dom.node) = [%bs.raw {| container.children |}];

  let (observer, observable) = Observable.create();

  let slides = Array.map(slide => {
    {
      isShown: false,
      node: slide,
      slideState: Inactive,
      heading: "",
      subheading: ""
    }
  }, slideNodes);

  clear({container, slides, observer, observable});
};
