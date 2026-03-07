import { get } from '../network/http'

import { getDocsUrl } from '../network/domain'

export function getDocs (fileName) {
  return get(`${getDocsUrl()}docs/md/deploy/${fileName}.json`)
}
