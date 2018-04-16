
let initialize = (selector: string, slideshowState: Slideshow.state) => {
  let container = Dom.querySelector(Dom.document, selector);
  let prevButton = Dom.querySelector(container, ".prev");
  let nextButton = Dom.querySelector(container, ".next");

  Dom.addEventListener(prevButton, () => 
};
