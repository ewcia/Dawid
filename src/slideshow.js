import {forEach} from 'ramda'
import EventEmitter from 'events'


export default class Slideshow extends EventEmitter {
  constructor(containerSelector) {
    super()

    this.container = document.querySelector(containerSelector)
    this.images = Array.prototype.slice.call(this.container.children)
    
    forEach(image => this.container.removeChild(image), this.images)
    this.inTransition = false

    this.clear = this.clear.bind(this)
    this.showImage = this.showImage.bind(this)
    this.transitionTo = this.transitionTo.bind(this)
    this.transitionBackwards = this.transitionBackwards.bind(this)
    this.transitionForwards = this.transitionForwards.bind(this)

    this.showImage(0)
  }

  clear() {
    if (this.container.children.length > 0) {
      forEach(image => this.container.removeChild(image), this.images)
    }
  }

  clearState() {
    forEach(image => {
      image.classList.remove('slide')
      image.classList.remove('slide-down')
    }, this.images)
  }

  showImage(idx) {
    this.clear()
    this.container.appendChild(this.images[idx % this.images.length])

    this.currentIdx = idx % this.images.length
    this.emit('imageShown', this.currentIdx)
  }

  transitionBackwards(idx) {
    this.inTransition = true
    this.container.insertBefore(this.images[idx % this.images.length], 
      this.container.children[0])

    forEach(child => child.classList.add('slide-down'), this.container.children)
    
    const animationEndCb = () => {
      this.container.removeChild(this.container.children[1])

      this.currentIdx = idx % this.images.length
      this.emit('imageShown', this.currentIdx)
      this.inTransition = false

      this.clearState()
      this.container.children[0].removeEventListener('animationend', animationEndCb)
    }
    
    this.container.children[0].addEventListener('animationend', animationEndCb, false)
  }

  transitionForwards(idx) {
    this.inTransition = true
    this.container.appendChild(this.images[idx % this.images.length])
    forEach(child => child.classList.add('slide'), this.container.children)
    
    const animationEndCb = () => {
      this.container.removeChild(this.container.children[0])

      this.currentIdx = idx % this.images.length
      this.emit('imageShown', this.currentIdx)
      this.inTransition = false

      this.clearState()
      this.container.children[0].removeEventListener('animationend', animationEndCb)
    }

    this.container.children[1].addEventListener('animationend', animationEndCb, false)
  }

  transitionTo(idx) {
    if (this.inTransition) {
      return
    }

    if (idx < this.currentIdx) {
      this.transitionBackwards(idx)
    } else if (idx > this.currentIdx) {
      this.transitionForwards(idx)
    }
  }


}


