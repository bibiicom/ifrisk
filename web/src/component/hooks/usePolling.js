import { useEffect, useRef, useState } from 'react'

import axios from 'axios'

export default ({ api, params, flag, interval }, pollingDeeps, clearDeeps = []) => {
  const cancel = useRef()
  const instance = useRef()
  const timer = useRef()
  const [result, setResult] = useState()
  const [loading, setLoading] = useState(false)

  useEffect(() => {
    setResult()
  }, clearDeeps)

  useEffect(() => {
    const request = async (once) => {
      try {
        const result = await api(params, { cancelToken: new axios.CancelToken(c => { cancel.current = c }) })
        setResult(result)
        if (!once) {
          timer.current = setTimeout(() => {
            if (flag) {
              request()
            }
          }, interval || 10000)
        }
      } catch (e) {
      } finally {
        setLoading(false)
        cancel.current = null
      }
    }

    const polling = (once) => () => {
      if (flag) {
        request(once)
      }
    }
    instance.current = polling(true)
    if (flag) {
      setLoading(true)
      polling()()
    }

    return () => {
      timer.current && clearTimeout(timer.current)
      cancel.current && cancel.current({ code: -100 })
      cancel.current = null
    }
  }, pollingDeeps)

  // console.log('result', result)
  return { loading, result, request: instance.current }
}
