export const DEFAULT_PAGE_SIZE = 10;

export const shorten = (value = "", head = 6, tail = 4) => {
  if (!value) return "";
  const v = String(value);
  if (v.length <= head + tail) return v;
  return `${v.slice(0, head)}...${v.slice(-tail)}`;
};

export const safeToNumber = (value) => {
  const n = Number(value);
  return Number.isFinite(n) ? n : 0;
};

const parseCsvLine = (line = "") => {
  const out = [];
  let cur = "";
  let inQuotes = false;
  for (let i = 0; i < line.length; i += 1) {
    const ch = line[i];
    if (ch === '"') {
      if (inQuotes && line[i + 1] === '"') {
        cur += '"';
        i += 1;
      } else {
        inQuotes = !inQuotes;
      }
      continue;
    }
    if (ch === "," && !inQuotes) {
      out.push(cur);
      cur = "";
      continue;
    }
    cur += ch;
  }
  out.push(cur);
  return out.map((v) => String(v ?? "").trim());
};

export const parseHistoryCsv = (csvText = "") => {
  const text = String(csvText || "").replace(/^\uFEFF/, "");
  const lines = text.split(/\r?\n/).map((l) => l.trim()).filter(Boolean);
  if (!lines.length) return [];

  const rows = [];
  for (const line of lines) {
    const cols = parseCsvLine(line);
    if (cols.length < 4) continue;

    const [timestampRaw, contractRaw, tokenIdRaw, txHashRaw] = cols;
    const tsLower = String(timestampRaw || "").toLowerCase();
    if (!timestampRaw || /时间戳|timestamp/.test(tsLower)) continue;

    const timestamp = safeToNumber(timestampRaw);
    const contract = String(contractRaw || "").toLowerCase();
    const tokenId = String(tokenIdRaw || "");
    const txHash = String(txHashRaw || "");

    rows.push({
      timestamp,
      contract,
      tokenId,
      txHash,
      key: `${txHash}-${contract}-${tokenId}-${timestamp}`,
    });
  }

  rows.sort((a, b) => (b.timestamp || 0) - (a.timestamp || 0));
  return rows;
};
