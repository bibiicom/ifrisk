
// export function isIp () {
//   const host = window.location.hostname
//   return !!(host.split('.')[0] || '').match('^[0-9]*$')
// }
export function isIp() {
  const host = window.location.hostname || "";
  // Treat explicit localhost and IPv6 loopback as local
  if (host === "localhost" || host === "::1" || host === "[::1]") return true;
  // IPv4 simple check
  return /^(?:\d{1,3}\.){3}\d{1,3}$/.test(host);
}
