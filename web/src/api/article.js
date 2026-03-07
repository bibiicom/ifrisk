import { get } from '../network/http'
import { getArticleUrl } from '../network/domain'

export function getAllArticles () {
  return get(`${getArticleUrl()}focus/articles.json`)
}

export function getArticleViews (path) {
  return get(`${getArticleUrl()}focus${path}`)
}
