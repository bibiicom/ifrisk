export const isSvgImage = (src) => {
  if (!src) return false
  return /^data:image\/svg\+xml/i.test(src) || /\.svg(\?|#|$)/i.test(src)
}

