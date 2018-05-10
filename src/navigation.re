let createDots = (container: Dom.node, slideshowState: Slideshow.state): unit => {
  let circle = Dom.querySelector(container, ".circle"); 
  let nextButton = Dom.querySelector(container, ".next");

  Belt.Range.forEach(1, Array.length(slideshowState.slides) - 1, (idx) => {
    let clonedCircle = Dom.cloneNode(circle, true);
    Dom.setAttribute(clonedCircle, "data-idx", string_of_int(idx));

    Dom.insertBefore(container, clonedCircle, nextButton);
  });
};

let updateSelectors = (prevButton: Dom.node, nextButton: Dom.node, 
                       state: Slideshow.state, idx: int): unit => {
  if (idx < 1) {
    prevButton##style #= "visibility: hidden";
  } else {
    prevButton##style #= "";
  };

  if (idx > Js_array.length(state.slides) - 2) {
    nextButton##style #= "visibility: hidden";
  } else {
    nextButton##style #= "";
  }
};

let currentIdx = (slides: array(Slideshow.slide)) => {
  Js_array.findIndex((s: Slideshow.slide) => s.isShown, slides);
};

let animationTimeout = 10.0 *. 1000.0;

type timeoutAnimation = {
  stopAnimation: ref(bool),
  pausedAt: ref(option(int)),
  unpausedAt: ref(option(int))
};

let startTimeoutAnimation = (container: Dom.node, 
                             state: ref(Slideshow.state)): timeoutAnimation => {
  let idx = currentIdx(state^.slides);

  Js_array.forEach(node => 
               Dom.setAttribute(node, "stroke-dashoffset", "0"), 
              Dom.querySelectorAll(container, ".circle circle")) |> ignore;

  let circle = if (idx === 0) {
    Dom.querySelector(container, ".circle circle");
  } else {
    Dom.querySelector(container, {j|.circle[data-idx="$idx"] circle|j});
  };

  let startedAt = Dom.createDate()##getTime();
  let stopAnimation = ref(false);
  let pausedAt = ref(None);
  let unpausedAt = ref(None);

  let rec requestFrameCb = () => {
    if (!stopAnimation^) {
      let currentTime = Dom.createDate()##getTime();

      let delta = switch (unpausedAt^) {
        | None => switch (pausedAt^) {
          | None => 0;
          | Some(t) => currentTime - t;
        }
        | Some(t) => t - Js_option.getWithDefault(0, pausedAt^);
      };

      let passedTime = currentTime - startedAt - delta;
      let progress = float_of_int(passedTime) /. animationTimeout *. 44.0;

      if (progress > 44.0) {
        Slideshow.transition(state^, (idx + 1) mod (Js_array.length(state^.slides))) |> ignore;
      } else {
        Dom.setAttribute(circle, "stroke-dashoffset", string_of_float(progress));
        Dom.window##requestAnimationFrame(requestFrameCb);
      }
    }
  };

  Dom.window##requestAnimationFrame(requestFrameCb);
  { stopAnimation, pausedAt, unpausedAt };
};

let initialize = (selector: string, slideshowState: Slideshow.state): unit => {
  let container = Dom.querySelector(Dom.document, selector);
  let prevButton = Dom.querySelector(container, ".prev");
  let nextButton = Dom.querySelector(container, ".next");
  let innerCircle = Dom.querySelector(container, ".inner-circle");

  createDots(container, slideshowState);

  let slideshowState = ref(slideshowState);
  let timeoutAnimation = ref(None);

  slideshowState^.observable##subscribe(. update => {
    switch (update) {
      | State(newState) => {
        slideshowState := newState;
        updateSelectors(prevButton, nextButton, newState, currentIdx(newState.slides));
        timeoutAnimation := Some(startTimeoutAnimation(container, slideshowState));
      }
      | AnimationStart(nextIdx, newState) => {
        slideshowState := newState;

        let yPosition = 24 + nextIdx * 20;
        innerCircle##style #= ("top: " ++ string_of_int(yPosition) ++ "px");
        Js_option.getExn(timeoutAnimation^).stopAnimation := true;
      }
      | MoveTo(nextIdx) => {
        let yPosition = 24 + nextIdx * 20;
        innerCircle##style #= {j|transition: none; top: $(yPosition)px|j};
      }
      | Error(e) => Js.log(e);
    };

  }, (_) => {()}, (_) => {()}) |> ignore;

  Dom.addEventListener(container, Dom.click, (e) => {
    let idx = currentIdx(slideshowState^.slides);

    let circleInPath = Js_array.find(el => {
      switch (el##classList) {
        | Some(_) => Dom.classListContains(el, "circle");
        | None => false;
      }
    }, e##path);
    
    if (e##target === prevButton) {
      Slideshow.transition(slideshowState^, idx - 1) |> ignore;
    } else if (e##target === nextButton) {
      Slideshow.transition(slideshowState^, idx + 1) |> ignore;
    } else {
      switch (circleInPath) {
        | Some(circle) => {
          switch (Dom.getAttribute(circle, "data-idx")) {
            | None => Slideshow.transition(slideshowState^, 0);
            | Some(idx) => Slideshow.transition(slideshowState^, int_of_string(idx));
          } |> ignore
        }
        | _ => ()
      }
    }
  }, false);

  Dom.addEventListener(container, Dom.mouseEnter, (_) => {
    switch (timeoutAnimation^) {
      | None => ();
      | Some(state) => {
          let paused = Js_option.getWithDefault(0, state.pausedAt^);
          let unpaused = Js_option.getWithDefault(0, state.unpausedAt^);

          state.pausedAt := Some(Dom.createDate()##getTime() - (unpaused - paused));
          state.unpausedAt := None;
      }
    } |> ignore;
  }, false);

  Dom.addEventListener(container, Dom.mouseLeave, (_) => {
    switch (timeoutAnimation^) {
      | None => ();
      | Some(state) => {
          switch (state.pausedAt^) {
            | None => ();
            | Some(_) => state.unpausedAt := Some(Dom.createDate()##getTime());
          };
      }
    } |> ignore;
  }, false);
};
