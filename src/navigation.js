import {forEach, range} from 'ramda'

export default class Navigation {

  constructor(containerSelector, slideshow) {
    this.container = document.querySelector(containerSelector)
    this.slideshow = slideshow

    const prevButton = this.container.querySelector('.prev')
    const nextButton = this.container.querySelector('.next')

    prevButton.addEventListener('click', () => {
      slideshow.transitionTo(slideshow.currentIdx - 1)
    }, false)
    nextButton.addEventListener('click', () => {
      slideshow.transitionTo(slideshow.currentIdx + 1)
    }, false)

    this.updateSelectors(slideshow.currentIdx)
    this.addDots()

    slideshow.on('imageShown', this.updateSelectors.bind(this))
  }

  addDots() {
    const nextButton = this.container.querySelector('.next')

    forEach(() => {
      const image = this.container.querySelector('.circle')
      this.container.insertBefore(image.cloneNode(true), nextButton)
    }, range(1, this.slideshow.images.length))
  }

  updateSelectors(currentIdx) {

    const prevButton = this.container.querySelector('.prev')
    const nextButton = this.container.querySelector('.next')

    if (currentIdx < 1) {
      prevButton.style = 'visibility: hidden'
    } else {
      prevButton.style = ''
    }

    if (currentIdx > this.slideshow.images.length - 2) {
      nextButton.style = 'visibility: hidden'
    } else {
      nextButton.style = ''
    }
  }
}
