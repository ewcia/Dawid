
let initialize = (selector: string, slideshowState: Slideshow.state): unit => {
  let container = Dom.querySelector(Dom.document, selector);
  let prevButton = Dom.querySelector(container, ".prev");
  let nextButton = Dom.querySelector(container, ".next");

  let slideshowState = ref(slideshowState);

  Dom.addEventListener(prevButton, Dom.click, () => {
    let currentIdx = Js_array.findIndex((s: Slideshow.slide) => s.isShown, slideshowState^.slides);
    slideshowState := Slideshow.transition(slideshowState^, currentIdx - 1);
  }, false);

  Dom.addEventListener(nextButton, Dom.click, () => {
    let currentIdx = Js_array.findIndex((s: Slideshow.slide) => s.isShown, slideshowState^.slides);
    slideshowState := Slideshow.transition(slideshowState^, currentIdx + 1);
  }, false);
};
